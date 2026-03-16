= Fremgangsmåte

Vi startet med en prototypefase der vi skrev nogenlunde fungerende kode for å forstå det utleverte rammeverket og driveren til heisen. Videre brukte vi V-modellen og erfaringene ifra prototypingen for å strukturere, planlegge og implementere den faktiske produksjonskoden.

Fra prototypefasen fant vi blandt annet at den utleverte driverkoden var ekstremt treg og til tider gjorde at heisen kjørte forbi en etasje før den registrerte den. Derfor implementerte vi @lst:DriverOpt i elevio_init funksjonen til elevio.c for å forbedre responsen på driveren.

#figure(
  raw(lang: "c", block: true, "
  int flag = 1;
  setsockopt(
    sockfd,
    IPPROTO_TCP,
    TCP_NODELAY,
    &flag,
    sizeof(flag)
  );
  "),
  caption: [Setter et flag i initialisering som gjør at TCP sender packets individuelt istedenfor å vente på flere packets.],
) <lst:DriverOpt>

Siden driveren kommuniserer med elevator serveren gjennom mange små packets over TCP så ville TCP automatisk vente på flere packets før den sender dem som en stor packet. Til vanlig er dette fornuftig siden packetsene vi sendte var ofte mindre enn overheadet som trengtes til TCP.

Etter prototypingen begynte vi med den pragmatiske V-modellen og per den så starter vi med kravspesifikasjonene til heisen der vi tar utgangspunkt i FAT-kravene gitt i oppgavebeskrivelsen og use-case diagrammet i @usecaseUML.

#figure(
  placement: auto,
  image("media/use_case.svg"),
  caption: [Use-case diagram],
  scope: "parent",
) <usecaseUML>

Tilstandsmaskinen som vi drøftet frem gjennom prototyping vises i @stateUML.

Ut ifra tilstandsmaskinen kan use-case diagrammet beskrives slikt:

=== Velg Etasje
#pad(left: 2em)[
  Pre-conditions:
  #pad(left: 2em)[
    Heisen er initialisert og 
    stopp-knappen er sluppet.
  ]
  Trigger:
  #pad(left: 2em)[
    Bestillingsknapp inne i heisen blir trykket.
  ]
  Suksess-scenario:
  #pad(left: 2em)[
    1. Heisens tilstand og motor settes til tilhørende rettning.

    2. Heisen ankommer etasjen.

    3. Heisen stopper.

    4. Heisen åpner døra i 3 sekunder.
  ]

  Utvidelser:
  #pad(left: 2em)[
    1a. Hvis stoppknappen trykkes følger suksess-scenario alternativ flow ifølge use-case til stopknappen.
  ]

  Suksessgaranti:
  #pad(left: 2em)[
    1a. Hvis stopp-knappen trykkes følger suksess-scenario alternativ flow ifølge use-case til stop-knappen.
  ]
]

#figure(
  placement: auto,
  image("media/State_diagram2.svg"),
  caption: [Førsteutkast av tilstands diagram],
  scope: "parent",
) <stateUML>

Når det gjelder heisens oppførsel bestemte vi at hvis den beveger seg i en rettning skal den fullføre alle ordre som er forran den i den rettningen, med prioritet av ordre i samme rettning (dvs at den ignorerer ordre i motsatt rettning hvis ordre i samme retning er tilgjengelig forran den). Og at den går i idle dersom den ikke har flere ordre i den rettningen, og at idle tilstand alltid ser etter nye ordre og går i rettningen til den første ordren den registrerer.

Med utgangspunkt i det oppsatte rammeverket vårt, oversatte vi og implementerte vi koden vår i C. 

Modulene vi endte opp med å bruke var `main.c` og `elevatorutils.c` der `main.c` inneholder tilstandsmaskinen og `elevatorutils.c` inkluderer alle hjelpefunksjoner. Som for eksempel @lst:checkOrders som skjekker om det er ordre i etasjene den er i, og fullfører den dersom den er gyldig, gitt kravene. 

Førsteutkastet av denne implementasjonen ga en nogenlunde fungerende kode. Hver implementerte modul ble tested og fungerte. Men den passerte ikke alle kravene til FAT testen. Spesifikt test S6 der Stopp tilstanden kunne huske dens forrige etasje, men på grunn av tilstands oppsettet, er den også avhengig av hvilken rettning tilstanden er i. Derfor endte vi opp med å legge til en til tilstand som beskriver når heisen er stoppet imellom to etasjer. 