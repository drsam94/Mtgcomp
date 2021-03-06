# MTG Complexity
An investigation into the complexity of Magic: the Gathering card text throughout history
### Table of Contents
**[Background](#background)**<br>
**[Results](#results)**<br>
**[TODO](#todo)**<br>
**[How to Use](#how-to-use)**<br>
**[License](#license)**<br>

### Background
A few weeks ago I came accross this article: https://pudding.cool/2017/05/song-repetition/, which shows, using LZW-compressibility as a computational metric of repititiveness, that the meme that pop music has become more reptitive over the years is indeed true. While compressibility is perhaps best seen as a measure of "repetitiveness," it seems to me that it is a reasonable measure for "complexity" of a magic set, as simple sets would be expected to have mechanics repeated across cards and less unique text, while a complex set would have lots of unique cards with unique text that couldn't be compressed away. This is certainly a very blunt measure of complexity, but I decided to run with it and see where it went.
### Results
The most basic computation was to see how compressibility varied with "time" over the history of magic's expansions (I didn't use real time in my measurements, instead just using the progression of magic expansions, i.e assuming that unit time passed between the release of each expansion). For this first computation, using the raw text of every card in every expansion, I got the following results:

![Raw Compressibility](https://github.com/drsam94/Mtgcomp/blob/master/out/raw_graph.png)

Magic set abbreviations are along the x-axis in chronological order, and on the y-axis is the compressibility of the set, computed as (bits compressed) / (bits raw). The trend line is drawn in green, and we see that the r-value is 0.44, so the r-squared is about .19. While this isn't the strongest correlation in the world, it is enough that it definitely supports the hypothesis that the general trend of text on magic cards is towards getting simpler. However, it is worth noting that what we are really interested in is magic card text has truly gotten simpler in a "dumbing down" sense, though the text has also just become more standardized and clean, especially from the early days of magic. Thus, if we rerun the same experiment, but instead using the oracle text of all cards, we get the following:

![Oracle Compressibility](https://github.com/drsam94/Mtgcomp/blob/master/out/raw_graph.png)

From this we see that when we shift to using the oracle text, our r-value drops to 0.37, which gives us an r-squared of about .14. So while this shows that some of the simplification comes from oracle, the effect is still noticable using only oracle text.

Another consideration I had was whether size of set mattered: naturally, a smaller set will have fewer cards, and less room for repitition. This hypothesis is somewhat backed up by the fact that the least compressible expansions were Arabian Nights (78 cards) and "Time spiral timeshifted" (which mtgjson classified as a different set and I didn't bother to special case it to include it in time spiral, and has 121 cards), both smaller than most small sets. And, on the otherhand, most of the sets that compressed well were indeed the "Large Sets" (first sets of a block). So, I decided to rerun the experiment, but using only a random sample of 100 cards from each set (arabian nightsstill would only have 78, but this should normalize things almost everywhere else). This gave the following:

![Normalized Compressibility](https://github.com/drsam94/Mtgcomp/blob/master/out/normalized_graph.png)

Interestingly, normalization, (while it changed the actual data) had no effect on the correlation when using the raw text. Using the Oracle text however:

![Normalized Compressibility](https://github.com/drsam94/Mtgcomp/blob/master/out/normalizedO_graph.png)

our r-value reduces to .27, giving an r-squared of only .07. This is still not completely trivial, but would render the effect of time rather small. Of course as the sample was random, results could change from run-to-run, but empiracally I got about the same results each time.
### TODO
1. Are there more meaningful ways to breakdown sets? Don't count core sets, count blocks as single units, ... there are many ideas here.
2. Would different compression algorithms meaningfully change the result? I chose LZW because it was easy to write a toy implementation of and its easy to grasp conceptually what its doing, but there are literally hundreds of compression algorithms that I could have used instead. One interesting option would to use a compression algorithm that is optimized for small data sizes and doing card-by-card compression.
3. Is there a better encoding scheme of the magic text? I used mtgjson's data almost raw, only doing some preprocessing to restrict to ascii characters. But it probably makes sense to capture the "true" size of magic data to consider all the special magic symbols as single characters, rather than character sequences like {U}. It may also make sense to make the compression case-insensitive.
4. Make graphs prettier.
5. Add support for batch-processing to the generation code: right now, every time the program is run, it has to re-read and process the json. This doesn't add too much overhead, but it probably adds something like a half-second overhead to each run, so it could get out of hand if a large number of graphs needed to be generated at once
### How to Use
Building requires g++7.1 (which is the newest release as of this writing) as the source uses multiple C++17 and std::experimental features (not because I think that's a good way of writing code for other people to use, but because I wanted to use this project to play with the features).

In order to generate the graphs, an installation of gnuplot is also necessary, and is only known to work with gnuplot 4.6 patchlevel 4, though probably other versions will work fine.

The data used by this project comes from mtgjson.com, and can be downloaded using the included scripts.

The full set of outputs used in this readme can be generated by running generateAll.sh.
### License
(c) 2017 Sam Donow, GNU GPL License.
contact: drsam94@gmail.com
Repo also inclues source from https://github.com/nlohmann/json, which is covered under the MIT License.
