PROGRAM NOTEX;
{ Testing logical calculations. }
VAR
  A: INTEGER;
BEGIN
  { Let's think about truth. }
  WRITE('What is truth?');
  READ (A);
  IF NOT A THEN
    WRITE('False! Caught you! Buy me a late now.')
  ELSE
    A := NOT (A<4)
  { Indeed, what is truth? Check the symbol table. }
END
