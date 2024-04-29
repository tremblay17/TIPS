PROGRAM THREEDIM;
{ Thinking about a cube with three nested loops! }
VAR
    XAXIS: INTEGER;
    YAXIS: INTEGER;
    ZAXIS: INTEGER;
    X: INTEGER;
    Y: INTEGER;
    Z: INTEGER;
BEGIN
    X := 0;
    Y := 0;
    Z := 0;
    WRITE('How big is your cube? (x,y,z)?');
    READ(XAXIS);
    READ(YAXIS);
    READ(ZAXIS);
    WHILE Z <ZAXIS
    BEGIN
        WRITE('Starting a new z-level');
        WHILE X<XAXIS
        BEGIN
            WHILE Y  <  YAXIS
            BEGIN
                WRITE(X);
                WRITE(Y);
                WRITE(Z);
                WRITE(' ');
                Y := Y + 1
            END;
            Y:=0;
            X:=X+1
        END;
        X:=0;
        Y:=0;
        Z:=Z + 1
    END;
    WRITE('-- Madam, your cube is ready now ---')
END
