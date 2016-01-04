epifortune
==========

A `fortune`-like program which provide quotes from
[Epiquote](http://epiquote.fr).

Details about available options can be obtained with the `--help` option.

Requirements
------------
- Unix-based OS
- GNU Make
- Clang compiler
- C99 Standard Library
- Wget
- An Internet connection

Running epifortune
------------------
Simply invoke the `epifortune` command with or without options.

Running epifortune without options will prompt a random quote from
[Epiquote](http://epiquote.fr), with its author, its context and its number,
word-wrapped at about 80 columns.

With some of the options listed with `--help`,
user can choose to display a particular quote, with its number, to hide some
informations about the quote or word-wrap at an other number of columns.

Known bugs
----------
- Word-wrapping with very low values (less than 20) may cause odd display. Long
words would not be cut and some lines containing those long words would not be
wrapped at the given number of columns.

Contributors
------------
- Meven 'mevouc' Courouble (courou\_m - EPITA 2019)

Licence
-------

Code under [Beerware](http://fr.wikipedia.org/wiki/Beerware) licence.
