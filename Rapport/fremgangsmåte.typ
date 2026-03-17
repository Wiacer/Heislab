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
  image("media/use_case.drawio.pdf"),
  caption: [Use-case diagram],
  scope: "parent",
) <usecaseUML>

Videre gjorde vi idemyldring for arkitekturdesign gjennom å vurdere kodene skrevet i prototypefasen der vi til slutt kom fram til hvordan bestillingssystemet og arkitekturen kunne mest fornuftig implementeres. Det var feks her at vi kom fram til at vi skulle bruke en tilstandsmaskin for sluttimplementasjonen og hvilke tilstander vi kom til å trenge som er illustrert i @stateUML.

Gitt tilstandsmaskinen kan use-case diagrammet beskrives slikt:
#block(breakable: false)[
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

    2a. Døra forblir åpen dersom det er en obstruksjon
  ]
]
]

#block(breakable: false)[
=== Stopp
#pad(left: 2em)[
  Pre-conditions:
  #pad(left: 2em)[
    Heisen er initialisert.
  ]
  Trigger:
  #pad(left: 2em)[
    Stoppknappen trykkes.
  ]
  Suksess-scenario:
  #pad(left: 2em)[
    1. Heisen stopper

    2. Alle bestillinger slettes.

    3. Døra åpnes dersom heisen er i en etasje
  ]

  Utvidelser:
  #pad(left: 2em)[
    1a. Døra forblir åpen intill 3 sekunder etter stoppknappen trykkes

    2a. Døra forblir åpen dersom det er en obstruksjon.
  ]
]
]

#block(breakable: false)[
=== Gå opp/ned
#pad(left: 2em)[
  Pre-conditions:
  #pad(left: 2em)[
    Samme som Velg etasje
  ]
  Trigger:
  #pad(left: 2em)[
    Knapp opp eller ned utenfor heisen blir trykket
  ]
  Suksess-scenario:
  #pad(left: 2em)[
    Samme som Velg etasje
  ]

  Utvidelser:
  #pad(left: 2em)[
    Samme som Velg etasje
  ]
]
]

#figure(
  placement: auto,
  image("media/State_diagram2.drawio.pdf"),
  caption: [Førsteutkast av tilstands diagram],
  scope: "parent",
) <stateUML>

Når det gjelder heisens oppførsel bestemte vi at hvis den beveger seg i en rettning skal den fullføre alle ordre som er forran den i den rettningen, med prioritet av ordre i samme rettning (dvs at den ignorerer ordre i motsatt rettning hvis ordre i samme retning er tilgjengelig forran den). Og at den går i idle dersom den ikke har flere ordre i den rettningen, og at idle tilstand alltid ser etter nye ordre og går i rettningen til den første ordren den registrerer.

Med utgangspunkt i det oppsatte rammeverket vårt, oversatte vi og implementerte vi koden vår i C. 

Koden vi endte opp med var delt i `main.c` og `elevatorutils.c` der `main.c` inneholder tilstandsmaskinen og `elevatorutils.c` inkluderer alle hjelpefunksjoner. Som for eksempel @lst:checkOrders som skjekker om det er ordre i etasjene den er i, og fullfører den dersom den er gyldig, gitt kravene. `elevatorutils.c` kan også videre deles inn i IO funksjoner som håndterer inputs og outputs og tilstandsmaskin funksjoner som håndterer logikken i tilstandsmaskinen. 

Førsteutkastet av denne implementasjonen ga en nogenlunde fungerende kode. Hver implementerte funksjon ble tested og fungerte. Men den passerte ikke alle kravene til FAT testen. Spesifikt test S6 der Stopp tilstanden kunne huske dens forrige etasje, men på grunn av tilstands oppsettet, er den også avhengig av hvilken rettningstilstand den var i. Derfor endte vi opp med å legge til en til tilstand som beskriver når heisen er blitt stoppet imellom to etasjer kalt `STOP_IDLE`.
