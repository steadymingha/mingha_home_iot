# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.

import numpy as np

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    with open('onoff_raw.txt','r') as f:
        lines = f.readlines()

    raw_signal = []
    for item_line in lines:
        if 'rawData[200]' in item_line:
            items = item_line.split('rawData[200]:')[-1].split(',')
            raw_signal.append(items)
        else:
            continue

    # sorting
    on_signal = np.zeros(len(raw_signal[0]), dtype=int)
    off_signal = np.zeros(len(raw_signal[0]), dtype=int)
    for i, col in enumerate(raw_signal):
        #comparison with 8 values
        ref_idx = 0
        ref_value = int(raw_signal[ref_idx][i])
        header_flag = 0
        for k in range(8):
            if ref_value == int(raw_signal[k][i]) and ref_idx != k:
                header_flag += 1

                # result
                if header_flag == 4: # number of same values reaches 5.
                    on_signal[i] = ref_value
                    off_signal[i] = ref_value
                    break
            if k == 7: # # couldn't find common value. move on to the next reference idx
                ref_idx += 1

test = 0












test = 0


# See PyCharm help at https://www.jetbrains.com/help/pycharm/
