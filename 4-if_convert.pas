PROGRAM CONVERT1;
{ Convert from Fahrenheit to Celsius! }
VAR
  F: REAL; { Fahrenheit }
  C: REAL; { Celsius }
BEGIN
  WRITE('Give me a temperature, in Fahrenheit pleeeze:');
  READ (F) ;
  IF F < -100 THEN
    BEGIN
  	  WRITE('Temp is too low!');
	    WRITE('Is it really that cold?');
      WRITE('Time to break out a Hot Toddy!')
    END
  ELSE
    BEGIN
      { Conversion formula }
      C := (F - 32.0)*(5.0/9.0);
      { Tell the user about it }
      WRITE ('Fahrenheit = ');
      WRITE (F);
      WRITE ('Celsius = ');
      WRITE (C)
    END
END
