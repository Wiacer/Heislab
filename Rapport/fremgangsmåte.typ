= Fremgangsmåte

Vi startet med en prototypefase der vi skrev nogenlunde fungerende kode for å forstå det utleverte rammeverket og driveren til heisen og brukte videre V-modellen for videreutvikling og opprydding av prototypen.

Fra prototypefasen fant vi blandt annet at de utleverte driverkodene var ekstremt trege og til tider gjorde at heisen kjørte forbi en etasje før den registrerte den. Derfor implementerte vi @lst:DriverOpt

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

Per den pragmatiske V-modellen så starter vi med kravspesifikasjonene til heisen der vi tar utgangspunkt i FAT-kravene gitt i oppgavebeskrivelsen og use-case diagrammet i @usecaseUML.

#figure(
  placement: auto,
  image("media/use_case.svg"),
  caption: [Use-case diagram],
  scope: "parent",
) <usecaseUML>

Videre gjorde vi idemyldring for arkitekturdesign gjennom å vurdere kodene skrevet i prototypefasen der vi til slutt bestemte oss for en tilstandsmaskin med tilstandene i henhold til @stateUML.

#figure(
  placement: auto,
  image("media/State_diagram2.svg"),
  caption: [Førsteutkast av tilstands diagram],
  scope: "parent",
) <stateUML>

Dersom heisen beveger seg i en rettning skal den fullføre alle ordre som er forran den i den rettningen, med prioritet av ordre i samme rettning. Og at den går i idle dersom den ikke har flere ordre i den rettningen, og at idle tilstand alltid ser etter nye ordre og går i rettningen til den første ordren den registrerer.

Med utgangspunkt i det oppsatte rammeverket vårt, oversatte vi og implementerte vi koden vår i C. 

Modulene vi endte opp med å bruke var `main.c` og `elevatorutils.c` der `main.c` inneholder tilstandsmaskinen og `elevatorutils.c` inkluderer alle hjelpefunksjoner. Som for eksempel @lst:checkOrders som skjekker om det er ordre i etasjene den er i, og fullfører den dersom den er gyldig, gitt kravene. 

Førsteutkastet av denne implementasjonen ga en nogenlunde fungerende kode. Hver implementerte modul ble tested og fungerte. Men den passerte ikke alle kravene til FAT testen. Spesifikt test S6 der Stopp tilstanden kunne huske dens forrige etasje, men på grunn av tilstands oppsettet, er den også avhengig av hvilken rettning tilstanden er i. Derfor endte vi opp med å legge til en til tilstand som beskriver når heisen er stoppet imellom to etasjer. 