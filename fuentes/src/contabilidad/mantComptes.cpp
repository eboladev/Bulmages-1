/****************************************************************************
** Form implementation generated from reading ui file 'mantComptes.ui'
**
** Created: mié nov 12 02:36:43 2003
**      by: The User Interface Compiler ($Id: mantComptes.cpp,v 1.2 2004/01/16 15:15:40 tborras Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "mantComptes.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qheader.h>
#include <qlistview.h>
#include <qtable.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

static const char* const image0_data[] = { 
"10 14 4 1",
"# c None",
". c #000000",
"b c #000083",
"a c #ffffff",
".......###",
".aaaaaa.##",
".aaaaaaa.#",
".ababbbaa.",
".aaaaaaaa.",
".ababbaba.",
".aaaaaaaa.",
".abaabbba.",
".aaaaaaaa.",
".ababbbba.",
".aaaaaaaa.",
".ababbbba.",
".aaaaaaaa.",
".........."};

static const char* const image1_data[] = { 
"32 32 49 1",
". c None",
"I c #000000",
"c c #a4aec5",
"i c #acb6cd",
"t c #a4aec5",
"x c #acb2cd",
"p c #acb2c5",
"v c #acb2cd",
"z c #a4aec5",
"H c #8395b4",
"L c #8b95b4",
"C c #9caac5",
"B c #9caacd",
"G c #b4c2e6",
"F c #b4c6de",
"K c #b4c6e6",
"E c #bdc6e6",
"J c #bdcae6",
"r c #c5cee6",
"n c #c5ceee",
"# c #c5d2e6",
"b c #c5d2ee",
"a c #cdd2ee",
"A c #cdd6ee",
"O c #cddaee",
"P c #cddaf6",
"M c #d5daee",
"N c #d5daf6",
"y c #d5def6",
"m c #d5e2f6",
"h c #dee2f6",
"D c #dee2ff",
"q c #dee6f6",
"s c #dee6ff",
"w c #deeaff",
"T c #e6e6f6",
"R c #e6e6ff",
"Q c #e6eaf6",
"g c #e6eaff",
"l c #e6eeff",
"S c #eeeeff",
"k c #eef2ff",
"o c #eef6ff",
"u c #f6f2ff",
"f c #f6f6ff",
"e c #f6faff",
"U c #f6ffff",
"j c #fffaff",
"d c #ffffff",
"...#aba#aba#aba#abc.............",
"...addddddddddefghbi............",
"...bddddddddddjklmnop...........",
"...addddddddddeflqrest..........",
"...#dddddddddddeuqadksv.........",
"...adddddddddddekg#ddfwx........",
"...bdddddddddddjflnfflqyz.......",
"...addddddddddddfkABCBCBCB......",
"...#ddddddddddddjkkgDrEFGHI.....",
"...addddddddddddeflgArJJKLI.....",
"...bdddddddddddddfkhAbarnHI.....",
"...addddddddddddekhyAAaAbLI.....",
"...#dddddddddddeushyyMNOPHI.....",
"...addddddddddekQRhhyyMyPLI.....",
"...bdddddddddeullggqshhyDHI.....",
"...addddddddfkSSllggqqhhmLI.....",
"...#dddddddeeffkklSggTgqRHI.....",
"...addddddejffofkkgSggqgsLI.....",
"...bdddddddjjeeffkkSSggggHI.....",
"...addddddddeeffkkSSlgggwLI.....",
"...#dddddddddeeffkkSSlgglHI.....",
"...addddddddejfekfkklSgggLI.....",
"...bdddddddddejfekukklSglHI.....",
"...addddddddeefeofkkllglgLI.....",
"...#dddddddedeefekfSkgSglHI.....",
"...addddddUdeefekfkklSgggLI.....",
"...bdddddddedejffkuSSgSglHI.....",
"...adddddddjeefekkkSllgggLI.....",
"...#dddddddedeeffkkSSgSglHI.....",
"...aHLHLHLHLHLHLHLHLHLHLHLI.....",
"....IIIIIIIIIIIIIIIIIIIIIII.....",
"................................"};

static const char* const image2_data[] = { 
"32 32 120 2",
".d c None",
"Qt c None",
".J c #000000",
".c c #a4aec5",
".j c #acb6cd",
".u c #a4aec5",
".y c #acb2cd",
".q c #acb2c5",
".w c #acb2cd",
".A c #a4aec5",
".I c #8395b4",
".M c #8b95b4",
".D c #9caac5",
".C c #9caacd",
".H c #b4c2e6",
".G c #b4c6de",
".L c #b4c6e6",
".F c #bdc6e6",
".K c #bdcae6",
"#O c #c50400",
".s c #c5cee6",
".o c #c5ceee",
".# c #c5d2e6",
".b c #c5d2ee",
"#P c #cd0000",
"#U c #cd0400",
"#I c #cd0c00",
"#N c #cd1000",
"#H c #cd1400",
"#J c #cd3031",
".a c #cdd2ee",
".B c #cdd6ee",
".7 c #cddaee",
".8 c #cddaf6",
"#C c #d51800",
"#B c #d51c00",
"#D c #d53c31",
"#z c #d54431",
"#Z c #d5595a",
"#V c #d55962",
"#Q c #d5818b",
"#b c #d5a5a4",
".z c #d5def6",
".n c #d5e2f6",
"#y c #de2400",
"#x c #de2800",
"#s c #de2c00",
"#r c #de3000",
"#t c #de4c31",
"#Y c #de999c",
".0 c #de9d94",
".S c #dea194",
"#f c #dea19c",
".6 c #deaaa4",
".i c #dee2f6",
".E c #dee2ff",
".r c #dee6f6",
".t c #dee6ff",
".x c #deeaff",
"#w c #e62c00",
"#a c #e63800",
".5 c #e63c00",
"#p c #e64820",
"#A c #e67d62",
"#T c #e69da4",
"#M c #e6a19c",
"#j c #e6a59c",
"#1 c #e6c2cd",
"#E c #e6deee",
"#k c #e6e6f6",
"#l c #e6e6ff",
".h c #e6eaff",
".m c #e6eeff",
".4 c #ee4000",
".Z c #ee4400",
"#W c #ee6131",
"#R c #ee6931",
".R c #ee7139",
"#G c #eeaaa4",
"#L c #eeae9c",
"#F c #eeaea4",
"#X c #eeb29c",
"#S c #eeb2a4",
"## c #eed2d5",
".W c #eed6d5",
"#u c #eee2f6",
"#q c #eee6f6",
"#n c #eeeeff",
".l c #eef2ff",
".p c #eef6ff",
".Y c #f64800",
".3 c #f64c00",
"#g c #f65500",
".X c #f65510",
".2 c #f65910",
"#K c #f68552",
"#m c #f6be9c",
"#e c #f6cac5",
".Q c #f6d2d5",
".1 c #f6d6d5",
"#0 c #f6dad5",
".v c #f6f2ff",
".g c #f6f6ff",
".f c #f6faff",
"#i c #ff5500",
"#h c #ff5900",
"#c c #ff5d00",
".T c #ff6100",
".U c #ff6500",
"#d c #ff7531",
"#. c #ff7d31",
".O c #ff8131",
".9 c #ffc69c",
".N c #ffc6a4",
"#o c #ffded5",
"#v c #ffe2d5",
".V c #fff6f6",
".P c #fffaf6",
".k c #fffaff",
".e c #ffffff",
"QtQtQtQtQtQtQt.#.a.b.a.#.a.b.a.#.a.b.a.#.a.b.c.d.d.d.d.d.d.d.dQt",
"QtQtQtQtQtQtQt.a.e.e.e.e.e.e.e.e.e.e.f.g.h.i.b.j.d.d.d.d.d.d.dQt",
"QtQtQtQtQtQtQt.b.e.e.e.e.e.e.e.e.e.e.k.l.m.n.o.p.q.d.d.d.d.d.dQt",
"QtQtQtQtQtQtQt.a.e.e.e.e.e.e.e.e.e.e.f.g.m.r.s.f.t.u.d.d.d.d.dQt",
"QtQtQtQtQtQtQt.#.e.e.e.e.e.e.e.e.e.e.e.f.v.r.a.e.l.t.w.d.d.d.dQt",
"QtQtQtQtQtQtQt.a.e.e.e.e.e.e.e.e.e.e.e.f.l.h.#.e.e.g.x.y.d.d.dQt",
"QtQtQtQtQtQtQt.b.e.e.e.e.e.e.e.e.e.e.e.k.g.m.o.g.g.m.r.z.A.d.dQt",
"QtQtQtQtQtQtQt.a.e.e.e.e.e.e.e.e.e.e.e.e.g.l.B.C.D.C.D.C.D.C.dQt",
"QtQtQtQtQtQtQt.#.e.e.e.e.e.e.e.e.e.e.e.e.k.l.l.h.E.s.F.G.H.I.JQt",
"QtQtQtQtQtQtQt.a.e.e.e.e.e.e.e.e.e.e.e.e.f.g.m.h.B.s.K.K.L.M.JQt",
"QtQtQtQtQtQtQt.b.e.e.e.e.N.O.P.e.e.e.e.e.e.g.Q.R.S.b.a.s.o.I.JQt",
"QtQtQtQtQtQtQt.a.e.e.e.N.T.U.O.V.e.e.e.e.f.W.X.Y.Z.0.a.B.b.M.JQt",
"QtQtQtQtQtQtQt.#.e.e.N.T.U.T.U.O.P.e.e.f.1.2.3.Z.4.5.6.7.8.I.JQt",
"QtQtQtQtQtQtQt.a.e.e.9.U.T.U.T.U#..V.f##.X.Y.Z.4.5#a#b.z.8.M.JQt",
"QtQtQtQtQtQtQt.b.e.e.e.N.U.T.U.T#c#d#e.2.3.Z.4.5#a#f.i.z.E.I.JQt",
"QtQtQtQtQtQtQt.a.e.e.e.e.9.U#c#c#g#g.3.Y.Z.4.5#a#f.r.i.i.n.M.JQt",
"QtQtQtQtQtQtQt.#.e.e.e.e.e.N#c#h#i.3.3.Z.4.5#a#j.h#k.h.r#l.I.JQt",
"QtQtQtQtQtQtQt.a.e.e.e.e.e.e#m#g.3.Y.Z.4.5#a#j#n.h.h.r.h.t.M.JQt",
"QtQtQtQtQtQtQt.b.e.e.e.e.e.e#o.2.3.Z.4.5#a#p#q#n#n.h.h.h.h.I.JQt",
"QtQtQtQtQtQtQt.a.e.e.e.e.e#o.2.Y.Z.4.5#a#r#s#t#u.m.h.h.h.x.M.JQt",
"QtQtQtQtQtQtQt.#.e.e.e.e#v.2.3.Z.4.5#a#r#w#x#y#z#u.m.h.h.m.I.JQt",
"QtQtQtQtQtQtQt.a.e.e.e#v.2.Y.Z.4.5#a#A#s#y#y#B#C#D#E.h.h.h.M.JQt",
"QtQtQtQtQtQtQt.b.e.e#v.2.3.Z.4.5#a#F.k#G#y#B#C#H#I#J#E.h.m.I.JQt",
"QtQtQtQtQtQtQt.a.e.e#K.Y.Z.4.5#a#L.f.g.f#M#C#N#I#O#P#Q.m.h.M.JQt",
"QtQtQtQtQtQtQt.#.e.e.V#R.4.5#a#S.e.f.f.g.f#T#I#U#P#V#n.h.m.I.JQt",
"QtQtQtQtQtQtQt.a.e.e.e.V#W#a#X.e.f.f.g.f.l.g#Y#P#Z#n.h.h.h.M.JQt",
"QtQtQtQtQtQtQt.b.e.e.e.e.V#0.e.f.e.f.k.g.g.l.v#1#n.h#n.h.m.I.JQt",
"QtQtQtQtQtQtQt.a.e.e.e.e.e.e.e.k.f.f.g.f.l.l.l#n.m.m.h.h.h.M.JQt",
"QtQtQtQtQtQtQt.#.e.e.e.e.e.e.e.f.e.f.f.g.g.l.l#n#n.h#n.h.m.I.JQt",
"QtQtQtQtQtQtQt.a.I.M.I.M.I.M.I.M.I.M.I.M.I.M.I.M.I.M.I.M.I.M.JQt",
"QtQtQtQtQtQtQt.d.J.J.J.J.J.J.J.J.J.J.J.J.J.J.J.J.J.J.J.J.J.J.JQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt"};

static const char* const image3_data[] = { 
"32 32 216 2",
".c c None",
"bv c None",
"bi c #000000",
"bq c #000000",
"#2 c #000020",
"bh c #000000",
"#1 c #000020",
"bc c #000000",
"bb c #000000",
"aa c #000020",
"ba c #000010",
"ab c #000018",
"al c #000020",
"a2 c #000018",
".G c #000000",
"b# c #000018",
"ad c #000018",
"ac c #000020",
"aB c #000018",
"am c #000020",
"an c #000018",
"aA c #000018",
"ao c #000020",
".p c #ac3000",
".P c #000000",
"#c c #000000",
"ak c #000000",
"bn c #000008",
"az c #000008",
"aT c #000010",
".x c #b44810",
".o c #e69d73",
".F c #cd5d20",
".b c #9caac5",
".h c #acb2c5",
".X c #a4aec5",
".O c #b4715a",
"b. c #7385a4",
"aS c #7b85a4",
"aK c #7b89ac",
"ay c #7b8dac",
"#l c #8395b4",
"bu c #8b95b4",
"#b c #8b99b4",
"#u c #8b99bd",
"bo c #946152",
"bj c #946d5a",
"bk c #9c858b",
".2 c #9caac5",
".3 c #9caacd",
"bp c #ac796a",
".1 c #b46952",
"#. c #b4b2c5",
"#a c #b4bede",
"bf c #bd7152",
"bl c #bd8573",
"## c #bdc6e6",
"#k c #bdcae6",
"#t c #bdcee6",
"#j c #c5815a",
"a6 c #c5856a",
"br c #c5958b",
"#s c #c5aeb4",
".a c #c5d2e6",
".# c #c5d2ee",
"aY c #cd9573",
"aj c #cd9583",
"bd c #cd9973",
"#Z c #cda19c",
"#G c #cda5a4",
"aJ c #cdd2e6",
"Qt c #cdd2ee",
"a9 c #cdd6e6",
"#B c #cdd6ee",
"#I c #cddaee",
"#Q c #d56d31",
"#A c #d5794a",
"aI c #d58d6a",
"a7 c #d5d2de",
"a1 c #d5d2ee",
"a0 c #d5d6e6",
".W c #d5d6ee",
"a8 c #d5dae6",
"#S c #d5daee",
"#J c #d5daf6",
"#R c #d5deee",
"#H c #d5def6",
"#8 c #de7939",
".l c #deae5a",
".r c #ded6d5",
"bg c #ded6e6",
"aR c #dedeee",
"aZ c #dedef6",
"bm c #dee2ee",
".g c #dee2f6",
"a# c #dee2ff",
"#9 c #dee6f6",
"#0 c #dee6ff",
".9 c #e66910",
".V c #e67120",
"av c #e6914a",
"aP c #e6dade",
"aw c #e6e2ee",
"a. c #e6e6ff",
"bs c #e6eaf6",
".k c #e6eaff",
"ax c #e6eeff",
"#r c #ee7918",
"aO c #ee9952",
"a3 c #eea15a",
".n c #eeae83",
".z c #eeba73",
".Q c #eec283",
"aQ c #eeeaff",
".f c #eeeeff",
".e c #eef2ff",
".q c #eef6ff",
"aU c #f6ae62",
"aL c #f6b652",
"#d c #f6ba20",
".I c #f6c608",
".w c #f6c6ac",
"#3 c #f6ce8b",
"ap c #f6d294",
".H c #f6eef6",
".j c #f6f6ff",
".i c #f6faff",
"bt c #f6ffff",
".N c #ff8518",
".U c #ff8920",
".8 c #ff8d18",
"#i c #ff9129",
"#F c #ff9139",
".M c #ff9510",
"#z c #ff9931",
".0 c #ff9d10",
"#q c #ff9d29",
"#Y c #ffa14a",
"#h c #ffaa18",
"#E c #ffaa39",
"#P c #ffaa4a",
".T c #ffae00",
".L c #ffb200",
"#y c #ffb229",
"#7 c #ffb25a",
".K c #ffb600",
".C c #ffb610",
".7 c #ffba08",
"#g c #ffba10",
"#X c #ffba52",
".s c #ffbe00",
".Z c #ffbe08",
"#p c #ffbe18",
"#O c #ffbe31",
".Y c #ffc220",
"au c #ffc273",
"#f c #ffc610",
"#D c #ffc631",
"#v c #ffc64a",
".D c #ffc652",
"ai c #ffc65a",
"aH c #ffc673",
"#U c #ffca29",
"#N c #ffca39",
"aC c #ffca52",
".m c #ffca8b",
".B c #ffce00",
"#6 c #ffce4a",
"#K c #ffce6a",
"aN c #ffce7b",
".E c #ffce94",
".S c #ffd200",
"#x c #ffd220",
"#W c #ffd241",
"af c #ffd24a",
"at c #ffd25a",
"aG c #ffd273",
"aX c #ffd294",
".4 c #ffd2a4",
"ah c #ffd652",
"as c #ffd662",
"#M c #ffda39",
"aF c #ffda62",
".6 c #ffde08",
"#n c #ffde18",
"aE c #ffde73",
"aW c #ffde8b",
"be c #ffdec5",
".R c #ffe200",
".5 c #ffe208",
"#o c #ffe218",
"#5 c #ffe252",
"#m c #ffe2c5",
".A c #ffe600",
".t c #ffe639",
"ar c #ffe662",
"aV c #ffe6ac",
".J c #ffea00",
"#e c #ffea10",
"#w c #ffea20",
"#C c #ffea31",
".u c #ffeac5",
"a5 c #ffead5",
"#L c #ffee39",
"#V c #ffee41",
"aq c #ffee6a",
".v c #ffeed5",
"a4 c #ffeede",
"#4 c #fff252",
"aM c #fff28b",
"#T c #fff2e6",
"ag c #fff662",
"aD c #fff67b",
"ae c #fff6f6",
".y c #fffaff",
".d c #ffffff",
"Qt.#Qt.aQt.#Qt.aQt.#Qt.aQt.#Qt.b.c.c.c.c.c.c.c.c.c.c.c.c.c.c.c.c",
".a.d.d.d.d.d.d.d.d.d.d.d.e.f.g.#.h.c.c.c.c.c.c.c.c.c.c.c.c.c.c.c",
"Qt.d.d.d.d.d.d.d.d.d.d.i.j.k.g.l.m.n.o.p.c.c.c.c.c.c.c.c.c.c.c.c",
".a.d.d.d.d.d.d.d.d.d.d.d.q.f.r.s.t.u.v.w.x.c.c.c.c.c.c.c.c.c.c.c",
"Qt.d.d.d.d.d.d.d.d.d.d.d.y.e.z.A.B.C.D.E.F.G.c.c.c.c.c.c.c.c.c.c",
".a.d.d.d.d.d.d.d.d.d.d.d.i.H.I.J.K.L.M.N.O.P.c.c.c.c.c.c.c.c.c.c",
"Qt.d.d.d.d.d.d.d.d.d.d.d.d.Q.R.S.L.T.U.V.W.X.c.c.c.c.c.c.c.c.c.c",
".a.d.d.d.d.d.d.d.d.d.d.d.i.Y.A.Z.L.0.N.1.2.3.2.c.c.c.c.c.c.c.c.c",
"Qt.d.d.d.d.d.d.d.d.d.d.d.4.5.6.7.C.8.9#.###a#b#c.c.c.c.c.c.c.c.c",
".a.d.d.d.d.d.d.d.d.d.d.d#d#e#f#g#h#i#j#k#####l#c.c.c.c.c.c.c.c.c",
"Qt.d.d.d.d.d.d.d.d.d.d#m#n#o.Y#p#q#r#s.#.##t#u#c.c.c.c.c.c.c.c.c",
".a.d.d.d.d.d.d.d.d.d.d#v#w#x.Y#y#z#A#B#B.a.##l#c.c.c.c.c.c.c.c.c",
"Qt.d.d.d.d.d.d.d.d.d.v#x#C#D#D#E#F#G#H#I#J#B#b#c.c.c.c.c.c.c.c.c",
".a.d.d.d.d.d.d.d.d.d#K#L#M#N#O#P#Q.g#R#H#S#H#l#c.c.c.c.c.c.c.c.c",
"Qt.d.d.d.d.d.d.d.d#T#U#V#W#N#X#Y#Z.g#0#H.g#H#u#c.c.c.c.c.c#1#2.c",
".a.d.d.d.d.d.d.d.d#3#4#5#6#6#7#8#9a.#9.g.ga##l#c.c.c#2aaabacad#2",
"Qt.d.d.d.d.d.d.daeafagahahai#7aj.fa..k#9a.#0#bakaaalacacamanaoab",
".a.d.d.d.d.d.d.dapaqarasatauavaw.kax#9.k#9.gayazacamanaAaAaAaAaB",
"Qt.d.d.d.d.d.d.daCaDaEaFaGaHaI.f.faxa.#S.WaJaKazaoaAaoaAaoaAaoaA",
".a.d.d.d.d.d.d.daLaMaEaEaNaOaPaQaR#SaJaJaJQtaSaTaAaAaAaAaAaAaAaA",
"Qt.d.d.d.d.d.d.daU.vaVaWaXaYaZ#S#Sa0.WaJa1QtaKazaoaAaoaAaoaAaBa2",
".a.d.d.d.d.d.d.da3a4.va5a6a7a8#Sa0#Sa9a0aJ#Bb.azaAaAaAaBb#babbbc",
"Qt.d.d.d.d.d.d.dbd.vbebfbgaRaR#S#Sa0#SaJ.WaJaKazaBb#babbbcbhbi.c",
".a.d.d.d.d.d.d.dbjbkblbmaRaR#S#Sa0#SaJa0aJ.Waybnbbbcbhbi.c.c.c.c",
"Qt.d.d.d.d.d.d.dbobpawbm.gaRaR#S#S.W#S#Ra..k#b#cbq.c.c.c.c.c.c.c",
".a.d.d.d.d.d.d.ybrbsaRbmaRaRa8#SaR.k.kax.k.k#l#c.c.c.c.c.c.c.c.c",
"Qt.d.d.d.d.d.d.d.d.i.y.i.i.e.f.f.eax.f.k.k.k#u#c.c.c.c.c.c.c.c.c",
".a.d.d.d.d.d.d.d.i.y.i.i.j.j.e.eax.f.kaxbsax#l#c.c.c.c.c.c.c.c.c",
"Qtbt.d.d.d.d.d.d.d.i.y.i.i.j.j.e.eax.f.kaQ.k#b#c.c.c.c.c.c.c.c.c",
".abu#lbu#lbu#lbu#lbu#lbu#lbu#lbu#lbu#lbu#lbu#l#c.c.c.c.c.c.c.c.c",
".c#c#c#c#c#c#c#c#c#c#c#c#c#c#c#c#c#c#c#c#c#c#c#c.c.c.c.c.c.c.c.c",
"bvbvbvbvbvbvbvbvbvbvbvbvbvbvbvbvbvbvbvbvbvbvbvbvbvbvbvbvbvbvbvbv"};


/*
 *  Constructs a PlaComptes as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
PlaComptes::PlaComptes( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl ),
      image0( (const char **) image0_data ),
      image1( (const char **) image1_data ),
      image2( (const char **) image2_data ),
      image3( (const char **) image3_data )
{
    if ( !name )
	setName( "mantComptes" );
    setIcon( image0 );
    setProperty( "PlaComptes", "Pl" );
    mantComptesLayout = new QGridLayout( this, 1, 1, 11, 6, "mantComptesLayout"); 

    Layout6 = new QHBoxLayout( 0, 0, 6, "Layout6"); 

    TextLabel1 = new QLabel( this, "TextLabel1" );
    Layout6->addWidget( TextLabel1 );

    Layout2 = new QHBoxLayout( 0, 0, 0, "Layout2"); 

    codigocuenta = new QLineEdit( this, "codigocuenta" );
    codigocuenta->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)0, 0, 0, codigocuenta->sizePolicy().hasHeightForWidth() ) );
    Layout2->addWidget( codigocuenta );
    Layout6->addLayout( Layout2 );

    Layout16 = new QHBoxLayout( 0, 0, 6, "Layout16"); 

    TextLabel2 = new QLabel( this, "TextLabel2" );
    Layout16->addWidget( TextLabel2 );

    desccuenta = new QLineEdit( this, "desccuenta" );
    Layout16->addWidget( desccuenta );
    Layout6->addLayout( Layout16 );

    mantComptesLayout->addLayout( Layout6, 1, 0 );

    TabWidget2 = new QTabWidget( this, "TabWidget2" );

    tab = new QWidget( TabWidget2, "tab" );
    tabLayout = new QGridLayout( tab, 1, 1, 11, 6, "tabLayout"); 

    ListView1 = new QListView( tab, "ListView1" );
    ListView1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, ListView1->sizePolicy().hasHeightForWidth() ) );
    ListView1->setResizePolicy( QScrollView::Manual );
    ListView1->setAllColumnsShowFocus( TRUE );
    ListView1->setShowSortIndicator( TRUE );
    ListView1->setRootIsDecorated( TRUE );

    tabLayout->addWidget( ListView1, 0, 0 );
    TabWidget2->insertTab( tab, QString("") );

    tab_2 = new QWidget( TabWidget2, "tab_2" );
    tabLayout_2 = new QGridLayout( tab_2, 1, 1, 11, 6, "tabLayout_2"); 

    tablacuentas = new QTable( tab_2, "tablacuentas" );
    tablacuentas->setNumRows( 3 );
    tablacuentas->setNumCols( 3 );
    tablacuentas->setReadOnly( TRUE );
    tablacuentas->setSelectionMode( QTable::SingleRow );

    tabLayout_2->addWidget( tablacuentas, 0, 0 );
    TabWidget2->insertTab( tab_2, QString("") );

    mantComptesLayout->addWidget( TabWidget2, 2, 0 );

    layout22 = new QHBoxLayout( 0, 0, 0, "layout22"); 

    toolButton6 = new QToolButton( this, "toolButton6" );
    toolButton6->setMinimumSize( QSize( 28, 28 ) );
    toolButton6->setIconSet( QIconSet( image1 ) );
    layout22->addWidget( toolButton6 );

    toolButton5_2 = new QToolButton( this, "toolButton5_2" );
    toolButton5_2->setMinimumSize( QSize( 28, 28 ) );
    toolButton5_2->setIconSet( QIconSet( image2 ) );
    layout22->addWidget( toolButton5_2 );

    toolButton5_2_2 = new QToolButton( this, "toolButton5_2_2" );
    toolButton5_2_2->setMinimumSize( QSize( 28, 28 ) );
    toolButton5_2_2->setIconSet( QIconSet( image3 ) );
    layout22->addWidget( toolButton5_2_2 );
    QSpacerItem* spacer = new QSpacerItem( 220, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout22->addItem( spacer );

    mantComptesLayout->addLayout( layout22, 0, 0 );

    layout23 = new QHBoxLayout( 0, 0, 6, "layout23"); 
    QSpacerItem* spacer_2 = new QSpacerItem( 581, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout23->addItem( spacer_2 );

    ToolButton12 = new QToolButton( this, "ToolButton12" );
    layout23->addWidget( ToolButton12 );

    mantComptesLayout->addLayout( layout23, 3, 0 );
    languageChange();
    resize( QSize(664, 496).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( ToolButton12, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( tablacuentas, SIGNAL( doubleClicked(int,int,int,const QPoint&) ), this, SLOT( dbtabla(int,int,int,const QPoint&) ) );
    connect( ListView1, SIGNAL( currentChanged(QListViewItem*) ), this, SLOT( listpulsada(QListViewItem*) ) );
    connect( ListView1, SIGNAL( currentChanged(QListViewItem*) ), this, SLOT( listpulsada(QListViewItem*) ) );
    connect( ListView1, SIGNAL( doubleClicked(QListViewItem*) ), this, SLOT( listdblpulsada(QListViewItem*) ) );
    connect( codigocuenta, SIGNAL( returnPressed() ), this, SLOT( return_codigo() ) );
    connect( codigocuenta, SIGNAL( textChanged(const QString&) ), this, SLOT( codigocambiado(const QString&) ) );
    connect( desccuenta, SIGNAL( returnPressed() ), this, SLOT( return_descripcion() ) );
    connect( desccuenta, SIGNAL( textChanged(const QString&) ), this, SLOT( descripcioncambiada(const QString&) ) );
    connect( toolButton5_2_2, SIGNAL( clicked() ), this, SLOT( editarcuenta() ) );
    connect( toolButton5_2, SIGNAL( clicked() ), this, SLOT( borrarcuenta() ) );
    connect( toolButton6, SIGNAL( clicked() ), this, SLOT( nuevacuenta() ) );
    connect( ListView1, SIGNAL( returnPressed(QListViewItem*) ), this, SLOT( return_descripcion() ) );

    // tab order
    setTabOrder( desccuenta, codigocuenta );
    setTabOrder( codigocuenta, ListView1 );
    setTabOrder( ListView1, TabWidget2 );
    setTabOrder( TabWidget2, tablacuentas );
}

/*
 *  Destroys the object and frees any allocated resources
 */
PlaComptes::~PlaComptes()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void PlaComptes::languageChange()
{
    setCaption( tr( "Plan de Cuentas" ) );
    QToolTip::add( this, trUtf8( "\x50\x6c\xc3\xa0\x20\x64\x65\x20\x63\x6f\x6d\x70\x74\x65\x73", "\x4d\x61\x6e\x74\x65\x6e\x69\x6d\x65\x6e\x74\x20\x64\x65\x6c\x20\x70\x6c\xc3\xa0\x20\x64\x65\x20\x63\x6f\x6d\x70\x74\x65\x73" ) );
    TextLabel1->setText( trUtf8( "\x43\xc3\xb3\x64\x69\x67\x6f\x20\x3a" ) );
    TextLabel2->setText( trUtf8( "\x44\x65\x73\x63\x72\x69\x70\x63\x69\xc3\xb3\x6e" ) );
    TabWidget2->changeTab( tab, tr( "Plan Contable" ) );
    TabWidget2->changeTab( tab_2, tr( "Subcuentas" ) );
    toolButton6->setText( QString::null );
    toolButton5_2->setText( QString::null );
    toolButton5_2_2->setText( QString::null );
    ToolButton12->setText( tr( "&Salir" ) );
}

void PlaComptes::borrarcuenta()
{
    qWarning( "PlaComptes::borrarcuenta(): Not implemented yet" );
}

void PlaComptes::codigocambiado(const QString&)
{
    qWarning( "PlaComptes::codigocambiado(const QString&): Not implemented yet" );
}

void PlaComptes::dbtabla(int,int,int,const QPoint&)
{
    qWarning( "PlaComptes::dbtabla(int,int,int,const QPoint&): Not implemented yet" );
}

void PlaComptes::descripcioncambiada(const QString&)
{
    qWarning( "PlaComptes::descripcioncambiada(const QString&): Not implemented yet" );
}

void PlaComptes::editarcuenta()
{
    qWarning( "PlaComptes::editarcuenta(): Not implemented yet" );
}

void PlaComptes::listdblpulsada(QListViewItem*)
{
    qWarning( "PlaComptes::listdblpulsada(QListViewItem*): Not implemented yet" );
}

void PlaComptes::listpulsada(QListViewItem*)
{
    qWarning( "PlaComptes::listpulsada(QListViewItem*): Not implemented yet" );
}

void PlaComptes::return_descripcion()
{
    qWarning( "PlaComptes::return_descripcion(): Not implemented yet" );
}

void PlaComptes::nuevacuenta()
{
    qWarning( "PlaComptes::nuevacuenta(): Not implemented yet" );
}

void PlaComptes::return_codigo()
{
    qWarning( "PlaComptes::return_codigo(): Not implemented yet" );
}

