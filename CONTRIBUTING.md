# Orion Engine Contributions

First of all, thank you for your interest in contributing to Orion Engine. However, I'm not looking for any
contributions for this project of mine, and it's not likely to change in the future. Sorry!

Below I've included a list of several reasons for why this is a case.

## Reason I. It's just a personal project of mine.

At the end of the day, it's just a personal project - I've created it, because I'm interested in game engines and how
they work; by accepting contributions from other people I don't learn anything myself. Also, it's a rather nice way to
show my skills ;)

Nonetheless, if you find a bug, I'm more than happy to read about it in the GitHub issues.

## Reason II. I plan to release commercial games using it.

If I ever release any commercial game using this engine, and people contributed code to this project, then I'd have to
either:

* Release source code according to the chosen [license](LICENSE.md), which **will** hurt my sales.
* Write a legally binding contract for all contributors to sign beforehand, which would essentially transfer all
  copyrights of their contributions to me; I find that rather exploitative.

This is where open-source licensing shows its limits to me. Yes, I can pick a better suited license for this project, or
dual-license or write an EULA instead, or... just not accept any contributions. The code is 'open' either way, you can
do all you want with it (given you follow the [license](LICENSE.md)).

## Reason III. Rampant LLM usage plaguing the industry.

This is a big one, and it's a hill I'm willing to die on (and one on which I can ramble on-and-on forever). So instead,
here are some few short\(-ish\) points:

* You are not only sucking out all the fun out of programming for yourself, but also for others (in that case that'd be
  me, who needs to review changes the LLM did (not you; mind you).
* INAL, but from my understanding, you cannot legally copyright ANY code LLM generated if it was trained on stolen data
  (which all the popular models did).
* LLMs don't have any context nor *understanding* of anything; they just predict (non-deterministically) the next most
  likely word (token) in a given sequence, which (by definition) means that you cannot precisely control what exactly it
  generates. It *might* generate passable code, but more often than not it's just plain bad.
* I'm not going to decipher (or risk) if the code I'm seeing is a LLM generated ~~code~~ slop that might be merged into
  the codebase. Sadly, there are some bad actors who try to hide their 'AI' usage and I'm not willing to deal with that.

For these reasons, this project is entirely written by hand (as it should be).
