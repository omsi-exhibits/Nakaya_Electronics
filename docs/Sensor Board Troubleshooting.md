# Nakaya Sensor Board Troubleshooting

## The Issue:
The reed switches are being too sensitive and are sensing when there is a magnet in the adjacent slot.
The switches and the magnets all have a bit of a wiggle room and in some cases thats margin compounds
to make the problem more prominent. The particular location and orientation of the magnets that are
next to each other causes different compounding problems.

## Solutions to Try:
Adding spacers to the screws to drop the sensors down a little bit.
Physically smaller magnets.
Reverse magnet orientations so that they create destructive interference

## Progress:
Adding spacers seemed to help with the false positives. I tried 2 washers making a distance of 3.5mm
But upon some inspection I realized that grouping the magnets together into clusters seems to work
even better. This reduces the effect of 2 magnets sandwiching a empty slot and causing it to trigger
via the magnetic field interaction of the two magnets.

More tests found that when several adjacent magnets are attracted towards each other they strengthen
the field on the edges and sometimes enough to trigger the sensors next to them. So they should
be inserted into the pucks in such a way that they all repel each other. 

## Final solution used
Grouping magnets together into one group and having every magnet repel the ones next to it. This way
they do not create any constructive interference and only trigger the sensors in directly below them.
