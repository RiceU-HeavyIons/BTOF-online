import sys
import os

if len(sys.argv) < 3 :
	print "Usage:\n exe infileA infileB"
	exit()

preamble = """#A line beginning with the # character is a comment
#Board#, Channel#  where Board# is 0-16 and Channel# is 0-31
#If Pedestal=-1, Pedestal is ignored and Gain value is valid
#If Pedestal>=0, Pedestal is valid and Gain value is ignored
#Board #  Channel#  #Pedestal #Gain\n"""

preambleEast = """#VP001 east TAC offset\n"""
preambleWest = """#VP002 west TAC offset\n"""
chan = """{EW:<10}{c:<10}{off:<10}{gain:<10}\n"""
TACchans = [4, 5, 6, 7, 12, 13, 14, 15, 20, 21, 22, 23, 28, 29, 30, 31]

eastA = []
westA = []

eastB = []
westB = []

# read in A
print "file:", sys.argv[1]
with open( sys.argv[1] ) as inA:
	for i in range(5):
		line=inA.next().strip()
		print line
	# skip the EAST preamble
	line=inA.next().strip()
	print line
	for tc in TACchans:
		line = inA.next().strip()
		print line
		EW, c, off, gain = [ x for x in line.split(" ") if x != '' and x != '\n' ]
		# print chan.format( EW=EW, c=c, off=off, gain=gain )
		eastA.append( int( off ) )
	# skip the WEST preamble
	line=inA.next().strip()
	print line
	for tc in TACchans:
		line = inA.next().strip()
		print line
		EW, c, off, gain = [ x for x in line.split(" ") if x != '' and x != '\n' ]
		# print chan.format( EW=EW, c=c, off=off, gain=gain )
		westA.append( int( off ) )

# read in B
print "file:", sys.argv[2]
with open( sys.argv[2] ) as inB:
	for i in range(5):
		line=inB.next().strip()
		print line
	# skip the EAST preamble
	line=inB.next().strip()
	print line
	for tc in TACchans:
		line = inB.next().strip()
		print line
		EW, c, off, gain = [ x for x in line.split(" ") if x != '' and x != '\n' ]
		# print chan.format( EW=EW, c=c, off=off, gain=gain )
		eastB.append( int( off ) )
	# skip the WEST preamble
	line=inB.next().strip()
	print line
	for tc in TACchans:
		line = inB.next().strip()
		print line
		EW, c, off, gain = [ x for x in line.split(" ") if x != '' and x != '\n' ]
		# print chan.format( EW=EW, c=c, off=off, gain=gain )
		westB.append( int( off ) )


east = []
west = []
for a, b in zip( eastA, eastB ):
	nv = a+b
	if nv > 4096:
		nv = 4096
	if nv < 0:
		print "TAC OFFSETS MUST BE POSITVE"
	east.append( nv )

for a, b in zip( westA, westB ):
	nv = a+b
	if nv > 4096:
		nv = 4096
	if nv < 0:
		print "TAC OFFSETS MUST BE POSITVE"
	west.append( nv )

# Write parameters out
# 
with open('combined_TAC.dat', 'w') as outf:
	outf.write( preamble )
	outf.write( preambleEast )
	for idx, tc in enumerate(TACchans):
		outf.write( chan.format( EW=3, c=tc, off=east[idx], gain=0 ) )
	outf.write( preambleWest )
	for idx, tc in enumerate(TACchans):
		outf.write( chan.format( EW=4, c=tc, off=west[idx], gain=0 ) )