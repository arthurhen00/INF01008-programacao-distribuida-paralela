import sys
import random

def generate_points(seed, N):
    random.seed(seed)
    points = []
    for _ in range(N):
        x = random.randint(0, N)
        y = random.randint(0, N)
        points.append((x, y))
    return points

if __name__ == "__main__":
    if len(sys.argv) != 6:
        print("Usage: python generate_case.py <start_temp> <decay_rate> <seed> <N> <output_file>")
        sys.exit(1)

    start_temp = float(sys.argv[1])
    decay_rate = float(sys.argv[2])
    seed = int(sys.argv[3])
    N = int(sys.argv[4])
    output_file = sys.argv[5]

    points = generate_points(seed, N)

    with open(output_file, "w") as f:
        f.write(f"start_temp={start_temp}\n")
        f.write(f"decay_rate={decay_rate}\n")
        f.write(f"seed={seed}\n")
        f.write("points={" + ",".join(f"{{{x},{y}}}" for x, y in points) + "}\n")
    
    print(f"Test case written to {output_file}")