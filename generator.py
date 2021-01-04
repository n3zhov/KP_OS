import random
import string
import sys


def random_text(len_of_input):
    res = ''
    for i in range(len_of_input):
        res += random.choice(string.ascii_letters)
    return res


if __name__ == "__main__":
    test_file_name = "test.txt"
    len_of_text = int(sys.argv[1])
    text = random_text(len_of_text)
    with open(f'{test_file_name}', 'w') as ftest:
        ftest.write(f'{text}')

