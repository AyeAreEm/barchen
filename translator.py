from transformers import pipeline
import sys
import codecs
import pyperclip

pipe = pipeline("translation", model="google-t5/t5-base")

def main():
    with codecs.open("gen.txt", "w", "utf-8") as f:
        res = pipe(sys.argv[1])
        f.write(res[0]["translation_text"])
        pyperclip.copy(res[0]["translation_text"])

        f.close()

main()
