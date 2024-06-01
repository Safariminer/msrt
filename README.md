# MSRT
## ManiaScript Runtime

MSRT is an embeddable ManiaScript engine for application development and Trackmania debugging.

## Why does this exist?
I like ManiaScript. And I like programming in general. And I wanted ManiaScript to be more polyvalent and allow me to write actual programs instead of just gamemodes and Manialinks. Thus, I've started implementing a ManiaScript engine for my own purposes. Furthermore, I wanted to learn about language development. Keep in mind: the code is garbage.

## Does it work?
Hell yeah!

There's an example in the ``msrt`` folder called ``main.cpp`` that loads up a file called ``test.Script.txt`` that shows the engine working! ``test.Script.txt`` will keep getting bigger as I add more to the engine, but ``main.cpp`` will stay the same. To integrate MSRT in your project, you thus only have to include ``msrt_main.h`` and build the cpp files starting by ``msrt_``, and then use the only two functions present in the main function of the example to get ManiaScript in your software.

## What works?
- Strings
- String concatenation
- Logging
- that's it :/

## What doesn't?
- Basically everything else, but I'm working towards full ManiaScript compatibility.

## Will this project ever be completed?
I truly don't know. If enough people use it, probably, but productivity is a big word that I am not able to understand. Therefore, no promises.

## Can I contribute?
Sure, I guess. Beware: the code is pristine, premium, absolute pure ***garbage***.

## Can I use this in a project?
Sure! My intention with this was to use it for gamedev myself, so go ahead!