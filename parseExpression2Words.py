from num2words import num2words
import re

def parseExpression2Words(expression):
    # remove all spaces
    expression = "".join(expression.split())
    numOps = re.findall('[+-/*//()]|\d+', expression)

    newWords = []
    for numOp in numOps:
        if numOp == "+":
            newWords.append("plus")
        elif numOp == "-":
            newWords.append("minus")
        elif numOp == "*":
            newWords.append("times")
        elif numOp == "/":
            newWords.append("divided by")
        elif numOp == "(":
            newWords.append("left parenthesis")
        elif numOp == ")":
            newWords.append("right parenthesis")
        elif numOp.isdigit():
            newWords.append(num2words(int(numOp)))
        else:
            print("Error: unexpected characters found")
            return ""
    return " ".join(newWords)
