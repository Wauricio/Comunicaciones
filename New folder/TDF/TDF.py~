import sys
import math


def writeonf(arg,namefile):
	f=open(namefile,"wb")
	for x in arg:
		f.write(x+"\n")
	f.close()
	print("Exito! , el nombre del Archivo es :" + namefile)

def readSamples(fileName):
	arr=[]
	f = open(fileName, "r") 
	for line in f.readlines():
		arr.append(float(line[:-1]))
	return arr

def TDF(samples,fr,fi):
	T=len(samples)
	for x in range(0,T):
		temp=0
		for a in range(0,T):
			temp+=samples[a]*pow(math.e,(-1j*a*x*2*math.pi)/T)
		fr.write('%15f\n' % (temp.real))
		fi.write('%15f\n' % (temp.imag))

#def TDF(samples):
#	T=len(samples)
#	rsl=[]
#	for x in range(0,T):
#		temp=0
#		for a in range(0,T):
#			temp+=samples[a]*pow(math.e,(-1j*a*x*2*math.pi)/T)
#		rsl.append(temp)
#	return rsl	
			

def TDF2(samples,fr,fi):
	T=len(samples)
	max=0
	for x in range(0,T):
		temp=0
		for a in range(0,T):
			temp+=samples[a]*pow(math.e,(-1j*a*x*2*math.pi)/T)
		if temp>max:
			max=temp
		fr.write('%15f\n' % (temp.real))
		fi.write('%15f\n' % (temp.imag))

def main():
	if 	len(sys.argv)>=2:
		f=open("TDF-R"+sys.argv[1],"w")
		f2=open("TDF-I"+sys.argv[1],"w")
		TDF(readSamples(sys.argv[1]),f,f2)
	else:
		print ("Introduce el Nombre del Archivo de Entrada y Salida ")




if __name__ == '__main__':
	main()	



