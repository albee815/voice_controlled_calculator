import json
import sys
from num2words import num2words
from parseExpression2Words import parseExpression2Words

inf = sys.argv[1]
outf = sys.argv[2]

with open(inf) as data_file:
    with open(outf, 'wb') as outfile:
        data = json.load(data_file)
        if data['valid']:
            outfile.write("Your expression is ")
            outfile.write(parseExpression2Words(data['expression']))
            outfile.write(". Your result is ")
            outfile.write(num2words((data['result'])))
            outfile.write(".")
        else:
            outfile.write("Your expression is invalid.")
            outfile.write(data['errorInfo'])
            outfile.write(". Please try again!")
