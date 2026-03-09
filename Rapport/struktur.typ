= Struktur
Testing og opprydding av kode slik at den fungerer bedre og ser finere ut, i henhold til V-modellens prosess, ga det ferdige produktet beskrevet her.

Programmets funksjon kan beskrives som følger.

#figure(
  placement: auto,
  image("media/State_diagram_final.svg"),
  caption: [Fullført tilstandsdiagram],
  scope: "parent",
) <stateUMLFinal>

== Fellestrekk
Alle tilstander utenom stopp og init kaller @lst:updateIO, som oppdaterer indikator lys og ordre basert på inputs og sensorer. Og @lst:checkStop som endrer tilstand til Stopp dersom stoppknappen blir trykket.

Heisens tilstand beskrives med enumen @lst:elevState og structen @lst:programState 

== Initialiserings tilstand
Når programmet startes så skjekker heisen først etter gyldig etasje. Her tar den ingen inputs. Dersom den starter i en gyldig etasje går den umiddelbart til idle state. Hvis ikke så begynner heisen å bevege seg nedover intill den treffer en gyldig etasje.

== Idle tilstand
I idle tilstand vil programmet lese ordre listen intill den finner en ny ordre.  Dersom ordren er i etasjen den er i åpner den bare døren. Ellers vil den endre tilstanden sin til rettningen ordren kom fra.

== Rettnings tilstand
Dersom heisen er i en rettnings tilstand, altså opp eller ned, så vil motoren være i den korrosponderende rettningen. Dersom den er i en gyldig etasje vil den skjekke etter ordre. Først skjekker den alle ordre i rettningen den går i. Og dersom det er en i etasjen den er i kaller den @lst:completeOrder og deretter @lst:checkOrders som skjekker etter flere ordre i bevegelsesrettning og endrer tilstand til idle dersom det ikke er noen.

== Stopp tilstand
Stopp tilstanden begynner med å slette alle ordre og skru av indikatorlys tilhørende knapper og stopper motoren. En while løkke kjører så lenge stopp knappen er holdt og hindrer andre inputs. Når stopp knappen slippes vil den først skjekke om den er i en gyldig etasje, og dersom den er det kaller den @lst:completeOrder. Hvis ikke vil tilstanden endres til stopp idle tilstanden.

== Stopp idle tilstand
Denne tilstanden er et spesialtilfelle av idle tilstanden. Den fungerer på mange måter likt, med unntak av at den avhenger av tilstanden den var i før stopp knappen ble trykket på, for å bestemme rettningen den går når den får en ordre. Altså heisen vil alltid vite forrige etasjen den var i, men dersom den er mellom to etasjer så veit for eksempel at den er mellom 2. og 1. etasje dersom forrige etasje var 2 og tilstanden dens er ned. Denne informasjonen bruker den til å fungere virtuelt på samme måte som idle tilstanden (Forrige tilstandsvariabelen vil ikke oppdateres dersom den er i stopp idle når stopp knappen trykkes).

=== Kodesnutter
#figure(
  raw(lang: "c", block: true, "
  typedef enum {
    MV_DWN, 
    MV_IDLE, 
    MV_UP, 
    STOP, 
    STOP_IDLE, 
    INIT} ElevState;
  "),
  caption: [Tilstands enum],
) <lst:elevState>

#figure(
  raw(lang: "c", block: true, "
  struct ProgramState{
    ElevState elevatorState;
    ElevState previousState;
    int floor;
    int lastFloor;
    int floorLight;
    int orders[N_FLOORS][N_BUTTONS];
  };
  "),
  caption: [En struct som holder på alle variabler som beskriver heisens tilstand],
) <lst:programState>

#figure(
  raw(lang: "c", block: true, "
  void complete_order(
    struct ProgramState* programState
  );
  "),
  caption: [Stopper motoren, åpner døren og fullfører ordre i nåværende etasje],
) <lst:completeOrder>

#figure(
  raw(lang: "c", block: true, "
  void check_orders(
    struct ProgramState* programState
  );
  "),
  caption: [Leser gjennom ordre og endrer tilstand til idle dersom det ikke er flere ordre i bevegelsesrettning],
) <lst:checkOrders>

#figure(
  raw(lang: "c", block: true, "
  void update_IO(
    struct ProgramState* programState
  );
  "),
  caption: [Oppdaterer lys og ordre utifra inputs og sensorer],
) <lst:updateIO>

#figure(
  raw(lang: "c", block: true, "
  void check_stop(
    struct ProgramState* programState
  );
  "
  ),
  caption: [Umiddelbart endrer tilstand til stopp dersom stoppknappen blir trykket på], 
) <lst:checkStop>

