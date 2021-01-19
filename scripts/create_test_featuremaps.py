import deepdish as dd
import numpy as np
import random

def generate_activity_stream(size, activity, width):
    stream = []
    # mask for bitwidth
    mask = np.uint64( (1<<width)-1 )
    # create an initial value
    val_prev = random.getrandbits(width)
    # iterate over stream size
    for i in range(size):
        val = np.uint64(0) # random.randint(0,mask)
        # iterate over bits in word
        for j in range(width):
            # get the jth bit
            bit = np.uint64( np.uint64(val_prev) >> np.uint64(j) ) & np.uint64(1)
            # randomly switch bit based on activity
            if(random.random() < activity):
                val = np.bitwise_or( val, np.uint64( ( np.uint64(~bit) & np.uint64(1) ) << np.uint64(j) ) )
            else:
                val = np.bitwise_or( val, np.uint64( np.uint64(bit) << np.uint64(j) ) )
                #val |= bit << j
        stream.append(val&mask)
        val_prev = val&mask
    return np.array(stream)

if __name__ == "__main__":
    SIZE=10000
    WIDTH=64
    # create different streams
    data = {
        "activity_05" : generate_activity_stream(SIZE,0.05,WIDTH),
        "activity_10" : generate_activity_stream(SIZE,0.10,WIDTH),
        "activity_15" : generate_activity_stream(SIZE,0.15,WIDTH),
        "activity_20" : generate_activity_stream(SIZE,0.20,WIDTH),
        "activity_25" : generate_activity_stream(SIZE,0.25,WIDTH),
        "activity_30" : generate_activity_stream(SIZE,0.30,WIDTH),
        "activity_35" : generate_activity_stream(SIZE,0.35,WIDTH),
        "activity_40" : generate_activity_stream(SIZE,0.40,WIDTH),
        "activity_45" : generate_activity_stream(SIZE,0.45,WIDTH),
        "activity_50" : generate_activity_stream(SIZE,0.50,WIDTH),
        "activity_55" : generate_activity_stream(SIZE,0.55,WIDTH),
        "activity_60" : generate_activity_stream(SIZE,0.60,WIDTH),
        "activity_65" : generate_activity_stream(SIZE,0.65,WIDTH),
        "activity_70" : generate_activity_stream(SIZE,0.70,WIDTH),
        "activity_75" : generate_activity_stream(SIZE,0.75,WIDTH),
        "activity_80" : generate_activity_stream(SIZE,0.80,WIDTH),
        "activity_85" : generate_activity_stream(SIZE,0.85,WIDTH),
        "activity_90" : generate_activity_stream(SIZE,0.90,WIDTH),
        "activity_95" : generate_activity_stream(SIZE,0.95,WIDTH)
    }

    # save as a .h5 file
    dd.io.save("data/activity.h5", data)

