import csv
import matplotlib.pyplot as plt

def analyze_fps_log(log_file="fps_log.csv"):
    timestamps = []
    fps_values = []

    try:
        with open(log_file, 'r') as csvfile:
            csv_reader = csv.reader(csvfile)
            next(csv_reader, None)  # Skip header row
            for row in csv_reader:
                if row: # Check for empty rows
                    timestamp, fps = row
                    timestamps.append(float(timestamp))
                    fps_values.append(float(fps))
    except FileNotFoundError:
        print(f"Error: {log_file} not found. Please make sure the log file exists.")
        return

    if not fps_values:
        print("No FPS data found in the log file.")
        return

    avg_fps = sum(fps_values) / len(fps_values)
    min_fps = min(fps_values)
    max_fps = max(fps_values)

    print("\nFPS Statistics:")
    print(f"Average FPS: {avg_fps:.2f}")
    print(f"Minimum FPS: {min_fps:.2f}")
    print(f"Maximum FPS: {max_fps:.2f}")

    # Optional Plotting
    plt.figure(figsize=(10, 5))
    plt.plot(timestamps, fps_values, label='FPS')
    plt.xlabel('Timestamp (seconds)')
    plt.ylabel('FPS')
    plt.title('FPS over Time')
    plt.legend()
    plt.grid(True)
    plt.savefig('fps_plot.png')
    print("\nFPS plot saved as fps_plot.png")

if __name__ == "__main__":
    analyze_fps_log()
