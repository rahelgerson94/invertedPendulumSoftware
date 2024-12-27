import matplotlib.pyplot as plt
import pandas as pd
import sys
import os
from scipy.interpolate import make_interp_spline
import numpy as np

def plot_smooth_curve(file_path):
    """
    Reads a CSV file and creates a smooth curve for 'time' vs 'amplitude' data.

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

        # Generate smooth curve using spline interpolation
        time_new = np.linspace(time.min(), time.max(), 500)  # New time points
        spline = make_interp_spline(time, amplitude, k=3)  # Cubic spline
        amplitude_smooth = spline(time_new)

        # Create a figure for this file
        plt.figure(figsize=(8, 5))
        plt.plot(time_new, amplitude_smooth, color="blue", label="Smoothed Curve")

        # Add labels, title, legend, and grid
        plt.title(f"Smooth Curve: Time vs Amplitude ({os.path.basename(file_path)})")
        plt.xlabel("Time")
        plt.ylabel("Amplitude")
        plt.legend()
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
        plot_smooth_curve(file_path)

    # Show all figures at once
    plt.show()
