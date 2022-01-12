## Calibration

### Stepper labels
`0` - stepper moving device vertically<br>
`1` - stepper moving horizontal locking pin<br>
`true` - stepper moving clockwise<br>
`false` - stepper moving couner-clockwise<br>

### Stepper conection
`stepperCount` - amount of steppers<br>
`stepPin[stepperCount]` - step pins of every stepper controller<br>
`dirPin[stepperCount]` - direction pins of every stepper controller<br>
`limitSwitchPin[stepperCount]` - pins of limit switches (set as `INPUT_PULLUP`)<br>

### Calibration variables
`positionStepDifference` - amount of steps between two positions. (When distance between limit switch and first position is other than between other positions, place additional stepper `0` movement in `initialStepperCalibtation()` function. Example: `stepperMove(stepsCount, false, 0);`).<br>
`lockStepDifference` - steps amount needed to fully change horizontal locking pin position.<br>
`scale` - kilograms difference between two consecutive weight.
## Kalibracja:

### Oznaczenia silników
`0` - silnik poruszający urządzeniem w pionie<br>
`1` - silnik poruszający trzpieniem blokującym<br>
`true` - obrót silnika zgodnie z ruchem wskazówek zegara<br>
`false` - obrót silnika w kierunku przeciwnym do ruchu wskazówek zegara<br>
### Połączenia silników krokowych
`stepperCount` - ilość silników krokowych <br>
`stepPin[stepperCount]` - piny step ze sterownika kolejno dla każdego z silników <br>
`dirPin[stepperCount]` - piny kierunku ze sterownika kolejno dla każdego z silników<br>
`limitSwitchPin[stepperCount]` - piny wyłączników krańcowych kolejno dla każdego z silników<br>
### Zmienne kalibracyjne
`positionStepDifference` - ilość kroków między poszczególnymi obciążnikami <br> (W przypadku gdy od wyłącznika krańcowego do pierwszej pozycji ilość kroków jest inna niż między pozostałymi pozycjami, to w funkcji `initialStepperCalibration()` należy umieścić dodatkowy ruch na silniku `0`. Można to zrobić używając poniższej funkcji `stepperMove(róznica_w_krokach, false, 0);`)<br>
`lockStepDifference` - ilość kroków do całkowitego przesunięcia trzpienia blokującego <br>
`scale` - skala dla przelicznika kilogramów na pozycję - ilość kilogramów różnicy między dwoma kolejnymi obciążnikami.<br>
`maxPosition` - całkowita ilość obciążników.<br>

### dodatkowe informacje
Przy pierwszej kalibracji należy zweryfikować, czy we wszystkich linijkach kodu oznaczonych komentarzem `//TODO: validate direction` wartość kierunku (true lub false) jest poprawna.