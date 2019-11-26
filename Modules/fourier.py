import cv2
import numpy as np

def fourier(img_path):
	img = cv2.imread(img_path)
	img_gray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
	row, cols = img_gray.shape
	crow, ccol = row // 2, cols // 2
	fft = np.fft.fft2(img_gray)
	fft_shifted = np.fft.fftshift(fft)
	fft_shifted[crow - 75:crow + 75, ccol - 75:ccol + 75] = 0
	fft_inverse_shift = np.fft.ifftshift(fft_shifted)
	fft_inverse = np.fft.ifft2(fft_inverse_shift)
	fft_inverse = 20 * np.log(np.abs(fft_inverse))
	return np.mean(fft_inverse)