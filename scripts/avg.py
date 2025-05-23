#!/usr/bin/env python3
import sys

if len(sys.argv) < 2:
	print("You need to supply ./avg.py <input-files> <output-file>")
	sys.exit(1)

input_files = []
for i in sys.argv[1:len(sys.argv)-1]:
	input_files.append(i)

output_file=sys.argv[len(sys.argv)-1]


print(f"Gonna average these files: {input_files} -> {output_file}")


def process_line(l:str) -> list:
	ls=l.split("|")
	if len(ls) < 3 or l[0] != '|':
		return []
	
	data=[]
	i=1
	while i < len(ls)-1:
		data.append(ls[i])
		i += 1
	return data
	

def file_to_dict(f: str) -> list:
	out=[]
	with open(f, 'r') as file:
		for line in file:
			lp=process_line(line)
			if len(lp) == 0:
				continue
			out.append(lp)
	
	return out
	

def merge_rows(r: list) -> list:
	num_cols=len(r[0])
	col_to_merge=num_cols-1

	tmpl_row=r[0].copy()
	tmpl_row[col_to_merge] = 0;
	for row in r:
		print(row[col_to_merge])
		tmpl_row[col_to_merge] += float(row[col_to_merge])
	
	tmpl_row[col_to_merge] /= float(len(r));
	return tmpl_row
		
#reduces order
#Takes in a list of lists of lists, reduces to a list of lists
def average_dls(l:list) -> list:
	# remove header
	num_of_rows = len(l[0])-1
	num_of_cols = len(l[0][0])
	
	num_of_files = len(l);

	output_arr = []
	# add header to file
	output_arr.append(l[0][0])

	for r in range(1,num_of_rows):
		uncomb_rows=[]
		for fi in range(0,num_of_files):
			uncomb_rows.append(l[fi][r])
		row=merge_rows(uncomb_rows)
		output_arr.append(row)
			

	return output_arr
	
	

ds=[]
for f in input_files:
	d=file_to_dict(f);
	ds.append(d)

out_dict=average_dls(ds);
print(f"Post average:{out_dict}")

def postprocess(d:list) -> str:
	s=""
	for line in d:
		for e in line:
			s+='|'
			s+=str(e)
		s+="|\n"
	return s

with open(output_file, "w") as file:
    file.write(postprocess(out_dict))
