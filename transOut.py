import sys
input_file = sys.argv[1]
output_file = sys.argv[2]

mydict = {"zero":1,"one":1,"two":1,"three":1,"four":1,"five":1,"six":1,"seven":1,"eight":1,"nine":1,"ten":1,
"plus":1,"minus":1,"multiply":1,"divide":1}

with open(input_file, "r") as ifile:
    with open(output_file, "w") as ofile:
        first = True
        for line in ifile:
            items = line.split()
            for word in items:
                idx = mydict.get(word,-1)
                if idx == 1:
                    if first:
                        ofile.write(word)
                        first = False
                    else:
                        ofile.write(" "+word)
