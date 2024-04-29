PROGRAM AREAS;
{ TIPS program for computing some areas. }
VAR
    HEIGHT: REAL;
    WIDTH: REAL;
    PI: REAL;
    RAREA: REAL;
    EAREA: REAL;
BEGIN
    PI := 3.14159;
    WRITE('Input height: ');
    READ (HEIGHT);
    WRITE('Input width: ');
    READ (WIDTH);
    { area of rectangle }
    RAREA := HEIGHT * WIDTH;
    { area of ellipse; given semi-major / semi-minor axes }
    EAREA := PI * HEIGHT * WIDTH;
    WRITE('RAREA = ');
    WRITE(RAREA);
    WRITE('EAREA = ');
    WRITE(EAREA)
END