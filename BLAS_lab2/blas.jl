using CSV, FileIO, DataFrames, Statistics,Plots

data=CSV.read("time_csv_2.csv",delim=",",allowmissing=:none)

show(data)

mean_vectors=Float64[]
deviation_vectors=Float64[]
mean_matrix=Float64[]
deviation_matrix=Float64[]

for size in 1:51
    k=(size-1)*10+1
    l=size*10
    for i in 1:10
        push!(mean_vectors,mean(data[k:l,2]))
        push!(deviation_vectors,stdm(data[k:l,2],mean_vectors[size]))
        push!(mean_matrix,mean(data[k:l,3]))
        push!(deviation_matrix,stdm(data[k:l,3],mean_matrix[size]))
    end
end

p1=plot(data[1],mean_vectors,
    title="Mnozenie wektorow",
    label="czas",
    xlabel="rozmiar wektora",
    yerr=deviation_vectors)

p2=plot(data[1],mean_matrix,
    title="Mnozenie wektora\ni macierzy",
    label="czas",
    xlabel="rozmiar wektora",
    yerr=deviation_matrix)

plot(p1,p2,layout=2)
