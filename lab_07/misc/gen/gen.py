import sys
import random
import string

def generate_random_word(existing_words):
    word_length = random.randint(1, 100)
    letters = string.ascii_letters
    while True:
        word = ''.join(random.choice(letters) for _ in range(word_length))
        if word not in existing_words:
            if (len(sys.argv) == 3 and word[0] != sys.argv[2]) or len(sys.argv) < 3:
                existing_words.add(word)
                return word

def generate_random_file(file_path):
    if len(sys.argv) < 2:
        sys.exit(1)
    
    num_words = int(sys.argv[1])
    existing_words = set()
    with open(file_path, 'w') as file:
        for _ in range(num_words):
            word = generate_random_word(existing_words)
            file.write(word + '\n')

file_path = './misc/gen/generated.txt'
generate_random_file(file_path)