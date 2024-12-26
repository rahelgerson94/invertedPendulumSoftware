import matplotlib.pyplot as plt
import pandas as pd
import sys
import os

def plot_pulse_train(file_path):
    """
    Reads a CSV file and plots the 'time' vs 'amplitude' data as a pulse train.

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

        # Extract data
        time = data['time']
        amplitude = data['amplitude']

        # Plot the pulse train
        plt.figure(figsize=(8, 5))
        for t, a in zip(time, amplitude):
            plt.plot([t, t], [0, a], color="blue")  # Vertical line from (t, 0) to (t, a)

        # Add labels, title, and grid
        plt.title("Pulse Train: Time vs Amplitude")
        plt.xlabel("Time")
        plt.ylabel("Amplitude")
        plt.grid(True)

        # Display the plot
        plt.tight_layout()
        plt.show()

    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    # Check if the user provided the file path as a command-line argument
    if len(sys.argv) != 2:
        print("Usage: python3 plotData.py <path_to_csv_file>")
        sys.exit(1)

    # Get the file path from the command-line argument
    file_path = sys.argv[1]

    # Call the plotting function
    plot_pulse_train(file_path)
