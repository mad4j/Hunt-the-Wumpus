# Hunt-the-Wumpus

````
0010  REM- HUNT THE WUMPUS
0015  REM:  BY GREGORY YOB

0020  PRINT "INSTRUCTIONS (Y-N)";
0030  INPUT I$
0040  IF I$="N" THEN 52
0050  GOSUB 1000

0052  REM- ANNOUNCE WUMPUSII FOR ALL AFICIONADOS ... ADDED BY DAVE
0054  PRINT
0056  PRINT "     ATTENTION ALL WUMPUS LOVERS!!!"
0058  PRINT "     THERE ARE NOW TWO ADDITIONS TO THE WUMPUS FAMILY";
0060  PRINT " OF PROGRAMS."
0062  PRINT
0064  PRINT "     WUMP2:  SOME DIFFERENT CAVE ARRANGEMENTS"
0066  PRINT "     WUMP3:  DIFFERENT HAZARDS"
0067  PRINT

0068  REM- SET UP CAVE (DODECAHEDRAL NODE LIST)
0070  DIM S(20,3)
0080   FOR J=1 TO 20
0090    FOR K=1 TO 3
0100    READ S(J,K)
0110    NEXT K
0120   NEXT J
0130  DATA 2,5,8,1,3,10,2,4,12,3,5,14,1,4,6
0140  DATA 5,7,15,6,8,17,1,7,9,8,10,18,2,9,11
0150  DATA 10,12,19,3,11,13,12,14,20,4,13,15,6,14,16
0160  DATA 15,17,20,7,16,18,9,17,19,11,18,20,13,16,19

0170  DEF FNA(X)=INT(20*RND(0))+1
0180  DEF FNB(X)=INT(3*RND(0))+1
0190  DEF FNC(X)=INT(4*RND(0))+1

0200  REM-LOCATE L ARRAY ITEMS
0210  REM-1-YOU,2-WUMPUS,3&4-PITS,5&6-BATS
0220  DIM L(6)
0230  DIM M(6)
0240   FOR J=1 TO 6
0250   L(J)=FNA(0)
0260   M(J)=L(J)
0270   NEXT J

0280  REM-CHECK FOR CROSSOVERS (IE L(1)=L(2),ETC)
0290   FOR J=1 TO 6
0300    FOR K=J TO 6
0310    IF J=K THEN 330
0320    IF L(J)=L(K) THEN 240
0330    NEXT K
0340   NEXT J

0350  REM-SET# ARROWS
0360  A=5
0365  L=L(1)

0370  REM-RUN THE GAME
0375  PRINT "HUNT THE WUMPUS"
0380  REM-HAZARD WARNINGS & LOCATION
0390  GOSUB 2000

0400  REM-MOVE OR SHOOT
0410  GOSUB 2500
0420  GOTO O OF 440,480
0430  REM-SHOOT
0440  GOSUB 3000
0450  IF F=0 THEN 390
0460  GOTO 500
0470  REM-MOVE
0480  GOSUB 4000
0490  IF F=0 THEN 390
0500  IF F>0 THEN 550
0510  REM-LOSE
0520  PRINT "HA HA HA - YOU LOSE!"
0530  GOTO 560
0540  REM-WIN
0550  PRINT "HEE HEE HEE - THE WUMPUS'LL GETCHA NEXT TIME!!"
0560   FOR J=1 TO 6
0570   L(J)=M(J)
0580   NEXT J
0590  PRINT "SAME SET-UP (Y-N)";
0600  INPUT I$
0610  IF I$#"Y" THEN 240
0620  GOTO 360

1000  REM-INSTRUCTIONS
1010  PRINT "WELCOME TO 'HUNT THE WUMPUS'"
1020  PRINT "  THE WUMPUS LIVES IN A CAVE OF 20 ROOMS. EACH ROOM"
1030  PRINT "HAS 3 TUNNELS LEADING TO OTHER ROOMS. (LOOK AT A"
1040  PRINT "DODECAHEDRON TO SEE HOW THIS WORKS-IF YOU DON'T KNOW"
1050  PRINT "WHAT A DODECAHEDRON IS, ASK SOMEONE)"
1060  PRINT
1070  PRINT "     HAZARDS:"
1080  PRINT " BOTTOMLESS PITS - TWO ROOMS HAVE BOTTOMLESS PITS IN THEM"
1090  PRINT "     IF YOU GO THERE, YOU FALL INTO THE PIT (& LOSE!)"
1100  PRINT " SUPER BATS - TWO OTHER ROOMS HAVE SUPER BATS. IF YOU"
1110  PRINT "     GO THERE, A BAT GRABS YOU AND TAKES YOU TO SOME OTHER"
1120  PRINT "     ROOM AT RANDOM. (WHICH MIGHT BE TROUBLESOME)"
1130  PRINT
1140  PRINT "     WUMPUS:"
1150  PRINT " THE WUMPUS IS NOT BOTHERED BY THE HAZARDS (HE HAS SUCKER"
1160  PRINT " FEET AND IS TOO BIG FOR A BAT TO LIFT).  USUALLY"
1170  PRINT " HE IS ASLEEP. TWO THINGS WAKE HIM UP: YOUR ENTERING"
1180  PRINT " HIS ROOM OR YOUR SHOOTING AN ARROW."
1190  PRINT "     IF THE WUMPUS WAKES, HE MOVES (P=.75) ONE ROOM"
1200  PRINT " OR STAYS STILL (P=.25). AFTER THAT, IF HE IS WHERE YOU"
1210  PRINT " ARE, HE EATS YOU UP (& YOU LOSE!)"
1220  PRINT
1230  PRINT "     YOU:"
1240  PRINT " EACH TURN YOU MAY MOVE OR SHOOT A CROOKED ARROW"
1250  PRINT "   MOVING: YOU CAN GO ONE ROOM (THRU ONE TUNNEL)"
1260  PRINT "   ARROWS: YOU HAVE 5 ARROWS. YOU LOSE WHEN YOU RUN OUT."
1270  PRINT "   EACH ARROW CAN GO FROM 1 TO 5 ROOMS. YOU AIM BY TELLING"
1280  PRINT "   THE COMPUTER THE ROOM#S YOU WANT THE ARROW TO GO TO."
1290  PRINT "   IF THE ARROW CAN'T GO THAT WAY (IE NO TUNNEL) IT MOVES"
1300  PRINT "   AT RAMDOM TO THE NEXT ROOM."
1310  PRINT "     IF THE ARROW HITS THE WUMPUS, YOU WIN."
1320  PRINT "     IF THE ARROW HITS YOU, YOU LOSE."
1330  PRINT
1340  PRINT "    WARNINGS:"
1350  PRINT "     WHEN YOU ARE ONE ROOM AWAY FROM WUMPUS OR HAZARD,"
1360  PRINT "    THE COMPUTER SAYS:"
1370  PRINT " WUMPUS-  'I SMELL A WUMPUS'"
1380  PRINT " BAT   -  'BATS NEARBY'"
1390  PRINT " PIT   -  'I FEEL A DRAFT'"
1400  PRINT ""
1410  RETURN

2000  REM-PRINT LOCATION & HAZARD WARNINGS
2010  PRINT
2020   FOR J=2 TO 6
2030    FOR K=1 TO 3
2040    IF S(L(1),K)#L(J) THEN 2110
2050    GOTO J-1 OF 2060,2080,2080,2100,2100
2060    PRINT "I SMELL A WUMPUS!"
2070    GOTO 2110
2080    PRINT "I FEEL A DRAFT"
2090    GOTO 2110
2100    PRINT "BATS NEARBY!"
2110    NEXT K
2120   NEXT J
2130  PRINT "YOU ARE IN ROOM "L(1)
2140  PRINT "TUNNELS LEAD TO "S(L,1);S(L,2);S(L,3)
2150  PRINT
2160  RETURN

2500  REM-CHOOSE OPTION
2510  PRINT "SHOOT OR MOVE (S-M)";
2520  INPUT I$
2530  IF I$#"S" THEN 2560
2540  O=1
2550  RETURN
2560  IF I$#"M" THEN 2510
2570  O=2
2580  RETURN

3000  REM-ARROW ROUTINE
3010  F=0
3020  REM-PATH OF ARROW
3030  DIM P(5)
3040  PRINT "NO. OF ROOMS(1-5)";
3050  INPUT J9
3060  IF J9<1 OR J9>5 THEN 3040
3070   FOR K=1 TO J9
3080   PRINT "ROOM #";
3090   INPUT P(K)
3095   IF K <= 2 THEN 3115
3100   IF P(K) <> P(K-2) THEN 3115
3105   PRINT "ARROWS AREN'T THAT CROOKED - TRY ANOTHER ROOM"
3110   GOTO 3080
3115   NEXT K
3120  REM-SHOOT ARROW
3130  L=L(1)
3140   FOR K=1 TO J9
3150    FOR K1=1 TO 3
3160    IF S(L,K1)=P(K) THEN 3295
3170    NEXT K1
3180   REM-NO TUNNEL FOR ARROW
3190   L=S(L,FNB(1))
3200   GOTO 3300
3210   NEXT K
3220  PRINT "MISSED"
3225  L=L(1)
3230  REM-MOVE WUMPUS
3240  GOSUB 3370
3250  REM-AMMO CHECK
3255  A=A-1
3260  IF A>0 THEN 3280
3270  F=-1
3280  RETURN
3290  REM-SEE IF ARROW IS AT L(1) OR L(2)
3295  L=P(K)
3300  IF L#L(2) THEN 3340
3310  PRINT "AHA! YOU GOT THE WUMPUS!"
3320  F=1
3330  RETURN!
3340  IF L#L(1) THEN 3210
3350  PRINT "OUCH! ARROW GOT YOU!"
3360  GOTO 3270
3370  REM-MOVE WUMPUS ROUTINE
3380  K=FNC(0)
3390  IF K=4 THEN 3410
3400  L(2)=S(L(2),K)
3410  IF L(2)#L THEN 3440
3420  PRINT "TSK TSK TSK- WUMPUS GOT YOU!"
3430  F=-1
3440  RETURN
4000  REM- MOVE ROUTINE
4010  F=0
4020  PRINT "WHERE TO";
4030  INPUT L
4040  IF L<1 OR L>20 THEN 4020
4050   FOR K=1 TO 3
4060   REM- CHECK IF LEGAL MOVE
4070   IF S(L(1),K)=L THEN 4130
4080   NEXT K
4090  IF L=L(1) THEN 4130
4100  PRINT "NOT POSSIBLE -";
4110  GOTO 4020
4120  REM-CHECK FOR HAZARDS
4130  L(1)=L
4140  REM-WUMPUS
4150  IF L#L(2) THEN 4220
4160  PRINT "...OOPS! BUMPED A WUMPUS!"
4170  REM-MOVE WUMPUS
4180  GOSUB 3380
4190  IF F=0 THEN 4220
4200  RETURN
4210  REM-PIT
4220  IF L#L(3) AND L#L(4) THEN 4270
4230  PRINT "YYYIIIIEEEE . . . FELL IN PIT"
4240  F=-1
4250  RETURN
4260  REM-BATS
4270  IF L#L(5) AND L#L(6) THEN 4310
4280  PRINT "ZAP--SUPER BAT SNATCH! ELSEWHEREVILLE FOR YOU!"
4290  L=FNA(1)
4300  GOTO 4130
4310  RETURN
5000  END
````
