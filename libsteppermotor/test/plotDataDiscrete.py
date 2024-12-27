import argparse
import pandas as pd
import matplotlib.pyplot as plt

def plot_columns(file, columns):
    """
    Plots specified columns from a CSV file.

    Parameters:
        file (str): The path to the CSV file.
        columns (list of int): The list of column indices to plot.
    """
    # Read the CSV file into a DataFrame
    try:
        df = pd.read_csv(file)
    except FileNotFoundError:
        print(f"Error: File '{file}' not found.")
        return
    except Exception as e:
        print(f"Error reading file: {e}")
        return

    # Validate column indices
    max_index = len(df.columns) - 1
    for col in columns:
        if col < 1 or col > max_index:
            print(f"Error: Column index {col} is out of range. The file has {len(df.columns)} columns.")
            return

    # Get column names from the header row
    column_names = df.columns

    # Plot all specified columns in the same figure
    plt.figure()
    for col in columns:
        plt.plot(df.iloc[:, 0], df.iloc[:, col], label=f"{column_names[col]}")

    plt.title("Overlay Plot of Selected Columns")
    plt.xlabel(f"{column_names[0]}")
    plt.ylabel("Value")
    plt.legend()

    # Show the figure
    plt.show()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Plot specified columns from a CSV file.")
    parser.add_argument("filename", type=str, help="Path to the CSV file.")
    parser.add_argument("-c", "--columns", type=str, required=True,
                        help="Comma-separated list of column indices to plot (e.g., 1,2,3).")
    args = parser.parse_args()

    # Parse column indices
    try:
        column_indices = [int(i) for i in args.columns.split(",")]
    except ValueError:
        print("Error: Column indices must be integers separated by commas.")
        exit(1)

    # Plot the columns
    plot_columns(args.filename, column_indices)
