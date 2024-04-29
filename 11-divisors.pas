PROGRAM FACTORS;
{ Tell us about the FACTORS of your number! }
VAR
  I: INTEGER;
  N: INTEGER;
  MODULA: INTEGER;
  NUM: INTEGER;
BEGIN
  I := 1;
  WRITE('Give me a number, please:');
  READ(NUM);
  WRITE('');
  WRITE('The factors are:');

  WHILE (I<NUM+1)
  BEGIN
    MODULA := NUM;
    WHILE (MODULA > I)
      MODULA := MODULA - I;
    IF -MODULA = -I THEN
      WRITE(I);
    I := I + 1
  END;  
  
  WRITE('--- Sir, your factors are ready ---')
END
