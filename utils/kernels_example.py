#Define the kernel
def volume(radius, volume):
	volume = (4/3 * math.pi) * (radius ** 3)

# void saxpySerial(int N,
#                        float scale,
#                        float X[],
#                        float Y[],
#                        float result[])
# {

#     for (int i=0; i<N; i++) {
#         result[i] = scale * X[i] + Y[i];
#     }
# }

def saxpy(scale, x, y, result):
	result = scale * x + y

