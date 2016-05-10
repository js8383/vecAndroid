#Define the kernels
def volume(radius, volume):
	volume = (4/3 * math.pi) * (radius ** 3)

def saxpy(scale, x, y, result):
	result = scale * x + y

def sqrt(initialGuess, x, y):
  guess = initialGuess
  error = abs(guess * guess * x - 1.0)
  while (error > 0.00001):
    guess = (3.0 * guess - x * guess * guess * guess) * 0.5
    error = abs(guess * guess * x - 1.0)
  y = x * guess

# print(sqrt(1.0, 2.0, 10000))


# Cited from VecPy
def mandelbrot(row, col, count, 
               max: 'uniform', w_m1: 'uniform', h_m1: 'uniform',
               left: 'uniform', right: 'uniform', top: 'uniform', bottom: 'uniform'):
    x0 = left + col * (right - left) / w_m1
    y0 = bottom + (h_m1 - row) * (top - bottom) / h_m1
    x = y = count = 0
    xx, yy = (x * x), (y * y)
    #Standard escape time
    while (count < max and xx + yy < 16):
      x, y = (xx - yy + x0), (2 * x * y + y0)
      xx, yy = (x * x), (y * y)
      count += 1
    #Smooth shading
    if count < max:
      count += 1 - math.log2(math.log2(xx + yy) / 2)
    return (row, col, count)