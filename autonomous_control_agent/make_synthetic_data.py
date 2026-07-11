import noise
import numpy as np
from pathfinding.core.grid import Grid, GridNode
from pathfinding.finder.a_star import AStarFinder

WALKABLE_INDICE=1
OBSTACLE_INDICE=0

def generate_obstacle_course(width:int=100, height:int=100, scale:float=0.2, threshold:float=0.55, x_offset:int=0, y_offset:int=0) -> np.matrix:
    """
    Generate a 2D obstacle map using Perlin noise.
    width, height: dimensions of the map
    scale: frequency of the noise (lower = smoother)
    threshold: cutoff for obstacles
    """

    obstacle_map = np.zeros((height, width), dtype=np.int32)

    for y in range(height):
        for x in range(width):

            indice_noise = noise.pnoise2((x+x_offset) * scale, (y+y_offset) * scale, octaves=2)
            # Normalize noise from [-1,1] to [0,1]
            indice_noise = (indice_noise + 1) / 2.0

            # Convert noise into obstacle presence
            obstacle_map[y, x] = WALKABLE_INDICE if indice_noise > threshold else OBSTACLE_INDICE

    return obstacle_map

def print_course(course) -> None:
    for row in course:
        print("".join("#" if cell == OBSTACLE_INDICE else "." for cell in row))

def print_obstacle_course_with_navigation_path(obstacle_course:np.matrix, navigation_path:list[tuple[int,int]]) -> None:

    navigation_path_coordinates:list[tuple[int,int]] = {(node.x, node.y) for node in navigation_path}

    for row_index, row in enumerate(obstacle_course):

        output:str = ""

        for column_index, indice_value in enumerate(row):

            if (column_index, row_index) in navigation_path_coordinates:
                output += "@"
            elif indice_value == OBSTACLE_INDICE:
                output += "#"
            else:
                output += "."

        print(output)

def find_obstacle_course_path(obstacle_course:np.matrix) -> list[GridNode]:

    grid = Grid(matrix=obstacle_course)

    path_finder = AStarFinder()

    start_node:GridNode = None
    end_node:GridNode = None

    for row_index, row in enumerate(obstacle_course):
        for column_index, indice_value in enumerate(row):
            # Set the starting position to the first "1" encountered from the start of the obstacle course, heading toward the end
            if indice_value == WALKABLE_INDICE:
                start_node = grid.node(column_index, row_index)
                break

        if start_node is not None:
            break

    for row_index, row in reversed(list(enumerate(obstacle_course))):
        for column_index, indice_value in reversed(list(enumerate(row))):
            # Set the ending position to the first "1" encountered from the end of the obstacle course, heading toward the start
            if indice_value == WALKABLE_INDICE:
                end_node = grid.node(column_index, row_index)
                break

        if end_node is not None:
            break
      
    # Find grid path between start and end nodes

    path, runs = path_finder.find_path(start_node, end_node, grid) # ignore the "runs" value because it is just a performance metric of the pathfinding algorithm

    return path

if __name__ == "__main__":
    
    obstacle_course = generate_obstacle_course(50,25,0.1,0.4,400,400)
    navigation_path = find_obstacle_course_path(obstacle_course)

    print_obstacle_course_with_navigation_path(obstacle_course, navigation_path)
