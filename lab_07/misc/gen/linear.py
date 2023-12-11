import sys
import itertools
import string

def generate_words(length):
    alphabet = string.ascii_lowercase
    for combination in itertools.product(alphabet, repeat=length):
        yield ''.join(combination)

def write_words_to_file(file_path, total_words):
    with open(file_path, 'w') as file:
        generated_words = 0; words_len = 1
        for _ in range(total_words):
            for word in generate_words(1):
                file.write('z' * (words_len - 1) + word + '\n')
                generated_words += 1
                if generated_words == total_words:
                    return
            words_len += 1

if len(sys.argv) < 2:
    sys.exit(1)

total_words = int(sys.argv[1])
file_path = './misc/measure/linear.txt'
write_words_to_file(file_path, total_words)
