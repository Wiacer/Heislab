#let month = ("januar", "februar", "mars", "april", "mai", "juni", "juli", "august",
              "september", "oktober", "november", "desember")

#align(center)[
  #v(1fr)
  #text(size: 28pt, weight: "bold")[Lab 2: Heisrapport]
  #v(12pt)
  #text(size: 16pt)[Eirik Stokker Aksdal, Víkingur Sigurðsson]
  #v(8pt)
  #text(size: 12pt)[Gruppe 11]
  #v(8pt)
  #let today = datetime.today()
  #text(size: 14pt)[#today.day(). #month.at(today.month()-1) #today.year()]
  #v(1fr)
]