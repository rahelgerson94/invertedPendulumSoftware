import matplotlib.pyplot as plt
import pandas as pd
import sys
import os

def plot_pulse_train(file_path):
    """
    Reads a CSV file and creates a figure for 'time' vs 'amplitude' data as a square wave.

    Parameters:
    - file_path: str, path to the CSV file
    """
    try:
        # Check if the file exists
        if not os.path.exists(file_path):
            raise FileNotFoundError(f"File not found: {file_path}")

        # Load the data from CSV
        data = pd.read_csv(file_path)

        # Ensure the necessary columns exist
        if 'time' not in data.columns or 'amplitude' not in data.columns:
            raise ValueError("CSV file must contain 'time' and 'amplitude' columns")

        # Ensure data is not empty
        if data.empty:
            raise ValueError("CSV file is empty or has no valid data.")

        # Extract data
        time = data['time'].tolist()
        amplitude = data['amplitude'].tolist()

        # Construct square wave points
        square_time = []
        square_amplitude = []

        for i in range(len(time) - 1):
            square_time.extend([time[i], time[i + 1]])
            square_amplitude.extend([amplitude[i], amplitude[i]])

        # Add a return-to-zero step for the last point
        square_time.extend([time[-1], time[-1]])
        square_amplitude.extend([amplitude[-1], 0])

        # Create a figure for this file
        plt.figure(figsize=(8, 5))
        plt.plot(square_time, square_amplitude, color="blue", drawstyle='steps-post')

        # Add labels, title, and grid
        plt.title(f"Square Wave: {os.path.basename(file_path)}")
        plt.xlabel("Time")
        plt.ylabel("Amplitude")
        plt.grid(True)

    except Exception as e:
        print(f"Error in {file_path}: {e}")

if __name__ == "__main__":
    # Check if at least one file path is provided
    if len(sys.argv) < 2:
        print("Usage: python3 plotData.py <path_to_csv_file1> <path_to_csv_file2> ...")
        sys.exit(1)

    # Iterate through all provided file paths and create a figure for each
    for file_path in sys.argv[1:]:
        plot_pulse_train(file_path)

    # Show all figures at once
    plt.show()
