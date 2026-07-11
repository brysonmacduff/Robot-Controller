import pandas as pd
import torch
import torch.nn as nn
from torch.utils.data import Dataset, DataLoader
import numpy as np

TRAINING_SET_FILE = "data/synthetic_training_set.csv"

# -----------------------------
# 1. Load and preprocess dataset
# -----------------------------
df = pd.read_csv(TRAINING_SET_FILE)

# Input columns
input_cols = [
    "AccelerationX", "AccelerationY", "AccelerationZ",
    "AngularVelocityX", "AngularVelocityY", "AngularVelocityZ",
    "BlockageMask"
]

# Convert blockage mask from hex string to integer if needed
df["BlockageMask"] = df["BlockageMask"].apply(
    lambda x: int(x, 16) if isinstance(x, str) else int(x)
)

X = df[input_cols].values.astype(np.float32)
y = df["MotionCommand"].values.astype(np.int64)

# Normalize inputs (except blockage mask)
for i in range(6):
    mean = X[:, i].mean()
    std = X[:, i].std() + 1e-6
    X[:, i] = (X[:, i] - mean) / std

# Scale blockage mask to [0,1]
X[:, 6] = X[:, 6] / (2**32 - 1)

# -----------------------------
# 2. Dataset + DataLoader
# -----------------------------
class RobotDataset(Dataset):
    def __init__(self, X, y):
        self.X = torch.tensor(X, dtype=torch.float32)
        self.y = torch.tensor(y, dtype=torch.long)

    def __len__(self):
        return len(self.X)

    def __getitem__(self, idx):
        return self.X[idx], self.y[idx]

dataset = RobotDataset(X, y)
loader = DataLoader(dataset, batch_size=32, shuffle=True)

# -----------------------------
# 3. Define neural network
# -----------------------------
class ControlNet(nn.Module):
    def __init__(self):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(7, 32),
            nn.ReLU(),
            nn.Linear(32, 16),
            nn.ReLU(),
            nn.Linear(16, 4)  # 4 motion commands
        )

    def forward(self, x):
        return self.net(x)

model = ControlNet()

# -----------------------------
# 4. Training setup
# -----------------------------
criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)

# -----------------------------
# 5. Training loop
# -----------------------------
epochs = 50
for epoch in range(epochs):
    total_loss = 0.0
    for batch_X, batch_y in loader:
        optimizer.zero_grad()
        outputs = model(batch_X)
        loss = criterion(outputs, batch_y)
        loss.backward()
        optimizer.step()
        total_loss += loss.item()

    print(f"Epoch {epoch+1}/{epochs}  Loss: {total_loss:.4f}")

# -----------------------------
# 6. Export trained model
# -----------------------------
torch.save(model.state_dict(), "model_weights.pth")

# Export TorchScript model for C++
example_input = torch.randn(1, 7)  # 7 input features
traced = torch.jit.trace(model, example_input)
traced.save("model_weights.pt")
