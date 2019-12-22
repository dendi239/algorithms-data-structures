# Algebra stuff

## class `Z<mod>`

Class `Z<mod>` incapsulates residue by given modulo. 
It supports any modulo you providde, but if you need to divide reisudes one by each other, you'll need to ensure that `gcd(modulo, denominator) = 1`.

Currently supported: 
- constructor, `operator int`
- `operator+=`, `-=`, `*=`, `/=`, `==`, `!=`, `<`

So you can operate with `Z`s like any other numbers without need to worry about performing operations by modulo at any specific point.
