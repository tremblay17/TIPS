PROGRAM LOOPEX;
{ A nice looping example. }
{ All multiples up to 12. }
VAR
  COUNT: INTEGER;
  MULTIPLE: INTEGER;
  BASE : INTEGER;
BEGIN
  COUNT := 1;
  MULTIPLE := 0;
  { Get the base number }
  WRITE('Enter the base integer:');
  READ(BASE);
  { Loop and print the multiples }
  WRITE('Here come your multiples!');
  WHILE COUNT < 12
  BEGIN
    MULTIPLE := COUNT * BASE;
    WRITE(MULTIPLE);
    COUNT := COUNT + 1
  END

END
