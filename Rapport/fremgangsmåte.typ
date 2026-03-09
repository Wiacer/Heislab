= Fremgangsmåte

Vi startet med en prototypefase der vi skrev nogenlunde fungerende kode for å forstå det utleverte rammeverket og driveren til heisen og brukte videre V-modellen for videreutvikling og opprydding av prototypen.

Fra prototypefasen fant vi blandt annet at de utleverte driverkodene var ekstremt trege og til tider gjorde at heisen kjørte forbi en etasje før den registrerte den. Derfor implementerte vi @lst:DriverOpt

#figure(
  raw(lang: "c", block: true, "
  int flag = 1;
  setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
  "),
  caption: [Setter et flag i initialisering som gjør at TCP sender packets individuelt istedenfor å vente på flere packets.],
) <lst:DriverOpt>

Per den pragmatiske V-modellen så starter vi med kravspesifikasjonene til heisen der vi tar utgangspunkt i FAT-kravene gitt i oppgavebeskrivelsen og use-case diagrammet i @usecaseUML.

#figure(
  image("media/use_case.svg"),
  caption: [Use-case diagram],
) <usecaseUML>

Videre gjorde vi idemyldring for arkitekturdesign gjennom å skrive diverse enkle koder der vi til slutt bestemte oss for en tilstandsmaskin som er videre forklart i struktur seksjonen.

(figur)



