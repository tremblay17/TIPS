PROGRAM LOOPEX2;
{ More multiplication tables! }
VAR
  COL: INTEGER;
  ROW: INTEGER;
  PRODUCT: INTEGER;
  MAXIMUM: INTEGER;

BEGIN
  COL := 0;
  ROW := 0;
  
  WRITE('Enter the maximum integer');
  WRITE('(it must be greater than 0):');
  READ(MAXIMUM);

  WRITE('Multiplication Table:');
  
  WHILE ROW < MAXIMUM
  BEGIN
    WRITE('Starting a new row');

    COL := 0;
    WHILE COL < MAXIMUM
    BEGIN
      PRODUCT := COL * ROW;
      IF PRODUCT<100 THEN
        WRITE(PRODUCT)
      ELSE
      BEGIN
        WRITE('Now that is a big number!')
      END;
      COL := COL+1
    END;

    ROW:=ROW+1

  END;
  WRITE('--- the end ---')

END
