# Jacobi Symbol

The Jacobi Symbol is a generalization of the Legendre symbol --

Legendre symbol for positve integers a, n,

(\frac{a}{p}) = 0 if a congruent to 0 mod p, 
 = 1 if for some integer x, a congruent to x^2 mod p,
 = -1 if there is no such integer x.
 
 > What's an example calculation? What are the applciations of such numbers?
 
 (p/p) = 0,
 (1/p) = 1, since (p-1)^2 = 1 mod p 
 (-1/p) = 
 (-1/5) = 1, 2^2 = 4 mod 5
 (-1/6) = 
 (-1/2) = (1/2) = 1
 (-1/4) = 
 (-1/7) = 
 
 $2*2*k - 1 = x^2$
 (-1/p) = 0, p odd, 1, p even. proof? 
 
 p = 2 * c for some number c, 
 x^2 = -1 + k * 2 * c for some x in 
 
 ---
 
 If Euler's criterion and Jacobi symbol differ, then n is composite.
 
 > Why?
 suppose p is an odd prime, let a be odd, then 
 a^ (p-1)/2 = 1 mod p if a is a quadratic residue mod p,
            = -1 mod p if a is a quadratic nonresidue mod p
            
using the Legendre symbol, equivalently, a^(p-1)/2 = (a/p)

How do you determine if a is a quadratic residue mod p quickly?
Start at 2 -- square 2 mod p -- 
 
create a dictionary of numbers that are quadratic residues mod p -- 

allocate two arrys of numbers of size p. 
Populate the first one with the numbers 0..p.
zero the second one.

for i in 0..p-1
    if arr1[i] is 0 continue
    x = i
    while x < p
        square x
        reduce x mod p
        arr2[x] = 1

    
    
 

