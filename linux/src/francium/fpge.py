head = open("francium-simple.mol.head", 'r');
middlefile = open("francium-simple.mol.middle",'r');
tail = open("francium-simple.mol.tail", 'r');
out = open("francium-simple-fpge.mol", 'w');

spec = open("floorplan.spec", 'r');

scalex = 1.5;
scaley = 1.5;

defined=[];

for i in spec:
  i=i.split();
  if(i[0]=="livingroom"): i[0]="liv_rm"; i.append("liv_num"); i.append(0);
  elif(i[0]=="diningroom"): i[0]="din_rm";i.append("din_num"); i.append(1);
  elif(i[0]=="bedroom"): i[0]="bed_rm";i.append("bed_num"); i.append(4);
  elif(i[0]=="bathroom"): i[0]="kit_rm";i.append("kit_num"); i.append(2);
  defined.append(i);
  
print defined;

out.write(head.read());
out.write("counter generate()\n{\n");

middle="counter j; j=0;\n";
for i in defined:
  middle+='scrib_floor.build(' + str(i[6]) + ',' + str(i[3]) +','+ str(i[4]) +');\n';
  middle+='scrib_floor.x_coord='+str(i[1])+';\n';
  middle+='scrib_floor.y_coord='+str(i[2])+';\n';
  middle+='<'+ str(i[0]) + "[" + str(i[6]) + "]> = <scrib_floor>; " + str(i[5]) + "="+ str(i[5]) +'+1;\n';
  middle+='type_queue[tq_num]='+ str(i[6]) +'; tq_num=tq_num+1;\n';
  middle+='j=0; elm_queue[eq_num]=0; eq_num=eq_num+1;\n';
    

  out.write(middle); middle="";
  out.write(middlefile.read());
  middlefile = open("francium-simple.mol.middle",'r');

out.write("} [0]\n");
out.write(tail.read());

out.close();
