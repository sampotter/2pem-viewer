import matplotlib.pyplot as plt
import numpy as np
import os.path
import sys
import time

if __name__ == '__main__':
    # Get path of raw file, exit if not enough arguments were passed.
    if len(sys.argv) < 3:
        sys.exit('Usage: %s <raw_file_path> <write_path>' % sys.argv[0])
    raw_path = os.path.relpath(sys.argv[1])
    write_path = os.path.relpath(sys.argv[2])

    # Set the frame rate and time-related values
    frame_rate = 30 # Hz
    frame_period = 1.0/frame_rate # secs

    # Create memory mapped file of RAW file
    repeat = 15000
    num_chans = 4
    dim_x = 512
    dim_y = 512
    dtype = np.uint16
    shape = (repeat, num_chans, dim_x, dim_y)
    fp = np.memmap(raw_path, dtype, shape=shape)

    ch = 1 # Use red channel
    for i in range(repeat):
        print('Writing frame %d from channel %d' % (i, ch))
        t0 = time.process_time()
        frame = fp[i, ch, :, :]
        frame.tofile(write_path)
        t1 = time.process_time()
        dt = t1 - t0
        assert(dt < frame_period)
        t_wait = frame_period - dt
        time.sleep(t_wait)
