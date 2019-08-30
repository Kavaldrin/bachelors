import argparse


def main():
    parser = argparse.ArgumentParser(description='This is a program for detecting whether image is blurred.')

    preprocessing = parser.add_argument_group('Preprocessing')
    # options for preprocessing such as adding blur, changing color space, etc
    preprocessing.add_argument('-b', '--blur', action='store_true', help='Randomly apply blur to random fraction of an image.')
    analytical = parser.add_argument_group('Analytical methods')
    # options for processing images that involve analitycal approach to detecting blur such as Fourier filter
    analytical.add_argument('-f', '--fourier', help='Applies Fourier\'s filter to detect if an image is blurred.')
    machine_learning = parser.add_argument_group('Machine learning methods')
    # options for processing images that involve machine learning methods for detecting blur
    machine_learning.add_argument('-ml', '--machine-learning', help='Applies machine learning model to detect blurred images.')

    options = parser.parse_args()


if __name__ == "__main__":
    main()

