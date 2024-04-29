PROGRAM BMICALC;
{ Calculate some BMI. }
VAR
  WEIGHT: INTEGER;
  HEIGHT: INTEGER;
  BMI: REAL;

BEGIN
  WEIGHT := 0;
  HEIGHT := 0;
  BMI := 0.0;
  
  WRITE('What is your weight (in pounds)?');
  READ(WEIGHT);

  WRITE('What is your height (in inches)?');
  READ(HEIGHT);
  
  BMI := (703 * WEIGHT) / (HEIGHT * HEIGHT);

  WRITE('YOUR BMI IS');
  WRITE(BMI);
  WRITE('');

  IF BMI > 24.99 THEN
    WRITE('You might be overweight!')
  ELSE
    IF BMI < 18.5 THEN
      WRITE('You might be underweight!')
    ELSE
    BEGIN
      WRITE('You have a normal weight!');
      WRITE('Now, eat some candy corn like Mike Leach.')
    END
END
