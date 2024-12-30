

from bizparse import Bizparse


def main():
    b = Bizparse()

    b.load_stop_words("StopWords_Generic.txt")
    b.load_key_words("keywords.txt")

    nvidia = b.load_text("1cbe8fe7-e08a-46e3-8dcc-b429fc06c1a4.pdf", label = "Nvidia", parser="sentence_parser")
    amd = b.load_text("0000002488-24-000012.pdf", label = "AMD", parser="sentence_parser")
    apple = b.load_text("Apple.pdf", label="Apple")
    boeing = b.load_text("BOEING.pdf", label="Boeing")
    etrade = b.load_text("Etrade.pdf", label="Etrade")

    label_lst = ['Nvidia', 'AMD']

    b.plot_sentence_parser_data(label_lst)

    b.commonwords_barchart()

    b.sentiment_histogram()

    b.wordcloud()

    b.sankey(250) # 250 is the amount of times a word must occur to be included in the chart

    b.comparison_bar("Apple", "Etrade")


if __name__ == "__main__":
    main()