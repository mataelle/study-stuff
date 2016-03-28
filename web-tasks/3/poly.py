import sys

class Polynomial:
    # '''
    # Class for single-variable polynomials
    # >>> print(Polynomial({1: 1}))
    # x
    # >>> Polynomial({1: 1})
    # Polynomial({1: 1})
    # >>> print(Polynomial({1: 1}) + Polynomial({2: 2}))
    # x+2x^2
    # >>> x = Polynomial({1: 1})
    # >>> print(x + Polynomial({1: 1}))
    # 2x
    # >>> a = 3 *  x ** 2 - 4 * x
    # >>> b = 5 * x ** 2 - 1
    # >>> print(a * b)
    # 4x-3x^2-20x^3+15x^4
    # >>> print(a)
    # -4x+3x^2
    # >>> print(b)
    # -1+5x^2
    # >>> c = 1 - x ** 2
    # >>> print(c)
    # 1-x^2
    # '''
    def __init__(self, coefs):
        self.d = {}
        for power, coef in coefs.items():
            if coef:
                self.d[power] = coef

    def __str__(self):
        if len(self.d) == 0 :
            print(0)
        else :
            resStr =  ''.join(['{0}{1}'.format( (("+" if coef > 0 else "-") if abs(coef) > 0 else "")+
                                                (str(abs(coef)) if (abs(coef)!=1 and power != 0) or power == 0 else ""),
                                                "x^"+str(power) if abs(power) > 1 else "" if power == 0 else "x")
                               for power, coef in self.d.items()])
            if resStr[0] == '+' : return resStr[1:]
            return resStr

    def __add__(self, y):
        if not isinstance(y, Polynomial):
            p = Polynomial(self.d)
            p.d[0] = p.d.get(0, 0) + y
            return p
        else:
            p = Polynomial(self.d)
            for power, coef in y.d.items():
                p.d[power] = coef + (p.d[power] if power in p.d.keys() else 0)
                if p.d[power] == 0:
                    del p.d[power]
            return p
    def __neg__(self):
        for power, coef in self.d.items():
            self.d[power] = - coef
        return Polynomial(self.d)
    def __sub__(self, y):
        return self + -y

    def __mul__(self, y):
        if not isinstance(y, Polynomial):
            if y == 0 : return Polynomial({})
            p = Polynomial(self.d)
            for power in p.d.keys():
                p.d[power]*=y
            return p
        else :
            p = Polynomial({})
            for power, coef in self.d.items():
                for power2, coef2 in y.d.items():
                    p += Polynomial({power+power2:coef*coef2})
            return p
    def __pow__(self, power, modulo=None):
        if isinstance(power,int):
            p = Polynomial(self.d)
            resP = Polynomial(self.d)
            for i in range(1,power):
                resP*=p
            return resP

    def __radd__(self, y):
        return self + y
    def __rsub__(self, y):
        return -self+y
    def __rmul__(self, y):
        return self*y


if __name__ == '__main__':
    for param in sys.argv : print(param)

    import doctest
    doctest.testmod()

    x = Polynomial({1: 1})
    workingString = "(x - 5,1)(x-1)"
    workingString = workingString.replace(" ", "").replace(",", ".").replace("^", "**").replace(")(", ")*(").replace("x(", "x*(")
    for i in range(0,10):
        workingString = workingString.replace(str(i)+"x", str(i)+"*x")
    print(eval(workingString))
