import os, sys, inspect

# path to the .pyd file
cmd_folder = 'C:/Users/vermosen/Documents/GitHub/twsapi/twsPython/'

if cmd_folder not in sys.path:
    sys.path.insert(0, cmd_folder)

from twsPython import example

ex = example()

ex.a(1)
ex.b(2)

print(ex.a(), ex.b())

test = ComboLeg()

print(ComboLeg.conId, ComboLeg.ratio)