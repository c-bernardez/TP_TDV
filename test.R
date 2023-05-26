setwd("/Users/macbookpro/Desktop/TP_TDV")
csv <- read.csv("gap.csv", header = TRUE, sep = ",")
funciones <- csv[2:7]
gap <- csv[8:10]

small <- gap[1:10, ]
medium <- gap[11:20, ]
large <- gap[21:30, ]
xl <- gap[31:40, ]


# Generate colors for each line
colors <- c("red", "blue", "green", "purple")

#grafico el valor de las métricas estudiadas con ambos métodos,
#a medida aumenta el tamaño de la entrada
par(mfrow = c(3, 1), mar = c(2, 2, 2, 2))

plot(funciones$f_obj.greedy, type = "l",
     col = colors[1], lwd = "2",
     xlab = "número de archivo",
     ylab = "valor de función objetivo")
lines(funciones$f_obj.mcf, col = colors[2], lwd = "2")
legend("topleft", legend = c("greedy", "batching"), col = colors[1:2], lwd = 2)

plot(funciones$tiempo.greedy, type = "l",
     col = colors[1], lwd = "2", ylim = c(0, max(funciones$tiempo.mcf)),
     xlab = "número de archivo",
     ylab = "tiempo de ejecución (microsegundos)")
lines(funciones$tiempo.mcf, col = colors[2], lwd = "2")
legend("topleft", legend = c("greedy", "batching"), col = colors[1:2], lwd = 2)

plot(funciones$dist_max.greedy, type = "l",
     col = colors[1], lwd = "2", ylim = c(0, max(funciones$dist_max.greedy)),
     xlab = "número de archivo",
     ylab = "distancia maxima recorrida en la asignación")
lines(funciones$dist_max.mcf, col = colors
[2], lwd = "2")
legend("topleft", legend = c("greedy", "batching"), col = colors[1:2], lwd = 2)

par(mfrow = c(1, 1))

df <- data.frame(
  z_f_obj_prom = c(mean(small$z_f_obj),
                   mean(medium$z_f_obj),
                   mean(large$z_f_obj),
                   mean(xl$z_f_obj)),
  z_tiempo_prom = c(mean(small$z_tiempo),
                    mean(medium$z_tiempo),
                    mean(large$z_tiempo),
                    mean(xl$z_tiempo)),
  z_d_max_prom = c(mean(small$z_d_max),
                   mean(medium$z_d_max),
                   mean(large$z_d_max),
                   mean(xl$z_d_max))
)

colors <- c("red", "blue", "green")


#grafico la mejora relativa de cada métrica
par(mfrow = c(3, 1), mar = c(2, 2, 2, 2))

plot(df$z_f_obj_prom, type = "l",
col = colors[1], xlab = "", ylab = "Mejora relativa", main = "Valor de la 
función objetivo", ylim = c(min(df$z_f_obj_prom), max(df$z_f_obj_prom)), 
lwd = "2")

plot(df$z_tiempo_prom, type = "l",
col = colors[2], xlab = "", ylab = "Mejora relativa", main = "Tiempo de 
ejecución", ylim = c(min(df$z_tiempo_prom), max(df$z_tiempo_prom)), lwd = "2")

plot(df$z_d_max_prom, type = "l",
col = colors[3], xlab = "", ylab = "Mejora relativa", main = "Distancia 
maxima recorrida por un taxi de la asignación",
ylim = c(min(df$z_d_max_prom), max(df$z_d_max_prom)), lwd = "2")

par(mfrow = c(1, 1))


#----------------------------------


#H0: z_f_obj = 0 (no hay mejora)
#H1: z_f_obj > 0 (hay mejora)

var_muestral_small <- var(small$z_f_obj)
media_muestral_small <- mean(small$z_f_obj)
t_obs <- (media_muestral_small - 0) / sqrt(var_muestral_small / 10)
p_valor_small_fobj <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor small para test de mejora relativa de la funcion objetivo")
print(p_valor_small_fobj)

var_muestral_medium <- var(medium$z_f_obj)
media_muestral_medium <- mean(medium$z_f_obj)
t_obs <- (media_muestral_medium - 0) / sqrt(var_muestral_medium / 10)
p_valor_medium_fobj <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor medium para test de mejora relativa de la funcion objetivo")
print(p_valor_medium_fobj)

var_muestral_large <- var(large$z_f_obj)
media_muestral_large<- mean(large$z_f_obj)
t_obs <- (media_muestral_large - 0) / sqrt(var_muestral_large / 10)
p_valor_large_fobj <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor large para test de mejora relativa de la funcion objetivo")
print(p_valor_large_fobj)

var_muestral_xl <- var(xl$z_f_obj)
media_muestral_xl <- mean(xl$z_f_obj)
t_obs <- (media_muestral_xl - 0) / sqrt(var_muestral_xl / 10)
p_valor_xl_fobj <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor xl para test de mejora relativa de la funcion objetivo")
print(p_valor_xl_fobj)

var_muestral <- var(gap$z_f_obj)
media_muestral <- mean(gap$z_f_obj)
t_obs <- (media_muestral - 0) / sqrt(var_muestral / 10)
p_valor_fobj <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor total para test de mejora relativa de la funcion objetivo")
print(p_valor_fobj)



#H0: z_tiempo = 0 (no hay mejora)
#H1: z_tiempo > 0 (hay mejora)

var_muestral_small <- var(small$z_tiempo)
media_muestral_small <- mean(small$z_tiempo)
t_obs <- (media_muestral_small - 0) / sqrt(var_muestral_small / 10)
p_valor_small_tiempo <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor small para test de mejora relativa del tiempo de ejecución")
print(p_valor_small_tiempo)

var_muestral_medium <- var(medium$z_tiempo)
media_muestral_medium <- mean(medium$z_tiempo)
t_obs <- (media_muestral_medium - 0) / sqrt(var_muestral_medium / 10)
p_valor_medium_tiempo <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor medium para test de mejora relativa del tiempo de ejecución")
print(p_valor_medium_tiempo)

var_muestral_large <- var(large$z_tiempo)
media_muestral_large<- mean(large$z_tiempo)
t_obs <- (media_muestral_large - 0) / sqrt(var_muestral_large / 10)
p_valor_large_tiempo <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor large para test de mejora relativa del tiempo de ejecución")
print(p_valor_large_tiempo)

var_muestral_xl <- var(xl$z_tiempo)
media_muestral_xl <- mean(xl$z_tiempo)
t_obs <- (media_muestral_xl - 0) / sqrt(var_muestral_xl / 10)
p_valor_xl_tiempo <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor xl para test de mejora relativa
relativa del tiempo de ejecución")
print(p_valor_xl_tiempo)

var_muestral <- var(gap$z_tiempo)
media_muestral <- mean(gap$z_tiempo)
t_obs <- (media_muestral - 0) / sqrt(var_muestral / 10)
p_valor_tiempo <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor total para test de mejora relativa del tiempo de ejecución")
print(p_valor_tiempo)



#H0: z_d_max = 0 (no hay mejora)
#H1: z_d_max > 0 (hay mejora)

var_muestral_small <- var(small$z_d_max)
media_muestral_small <- mean(small$z_d_max)
t_obs <- (media_muestral_small - 0) / sqrt(var_muestral_small / 10)
p_valor_small_dmax <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor small para test de mejora relativa de la distancia máxima")
print(p_valor_small_dmax)

var_muestral_medium <- var(medium$z_d_max)
media_muestral_medium <- mean(medium$z_d_max)
t_obs <- (media_muestral_medium - 0) / sqrt(var_muestral_medium / 10)
p_valor_medium_dmax <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor medium para test de mejora relativa de la distancia máxima")
print(p_valor_medium_dmax)

var_muestral_large <- var(large$z_d_max)
media_muestral_large<- mean(large$z_d_max)
t_obs <- (media_muestral_large - 0) / sqrt(var_muestral_large / 10)
p_valor_large_dmax <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor large para test de mejora relativa de la distancia máxima")
print(p_valor_large_dmax)

var_muestral_xl <- var(xl$z_d_max)
media_muestral_xl <- mean(xl$z_d_max)
t_obs <- (media_muestral_xl - 0) / sqrt(var_muestral_xl / 10)
p_valor_xl_dmax <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor xl para test de mejora relativa relativa de la distancia máxima")
print(p_valor_xl_dmax)

var_muestral <- var(gap$z_d_max)
media_muestral <- mean(gap$z_d_max)
t_obs <- (media_muestral - 0) / sqrt(var_muestral / 10)
p_valor_dmax <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor total para test de mejora relativa de la distancia máxima")
print(p_valor_dmax)


#----------------------------------


p_valores_fobj <- c(p_valor_small_fobj, p_valor_medium_fobj, 
p_valor_large_fobj, p_valor_xl_fobj)

p_valores_tiempo <- c(p_valor_small_tiempo, p_valor_medium_tiempo, 
p_valor_large_tiempo, p_valor_xl_tiempo)

p_valores_dmax <- c(p_valor_small_dmax, p_valor_medium_dmax, 
p_valor_large_dmax, p_valor_xl_dmax)

par(mfrow = c(3, 1), mar = c(2, 2, 2, 2))

plot(p_valores_fobj, xlab = "", ylab = "p-valor distancia maxima",
type = "l", lwd = 1, col = colors[1])
points(p_valores_fobj, col = colors[1], cex = 2)

plot(p_valores_tiempo, xlab = "", ylab = "p-valor distancia maxima",
type = "l", lwd = 1, col = colors[2])
points(p_valores_tiempo, col = colors[2], cex = 2)

plot(p_valores_dmax, xlab = "", ylab = "p-valor distancia maxima",
type = "l", lwd = 1, col = colors[3])
points(p_valores_dmax, col = colors[3], cex = 2)

par(mfrow = c(1, 1))


#----------------------------------



#H0: f_obj.greedy - f_obj.mcf = 0
#H0: f_obj.greedy - f_obj.mcf > 0
small2 <- csv[1:10, c(2, 5)]
var_muestral_small_greedy <- var(small2$f_obj.greedy)

var_muestral_small_mcf <- var(small2$f_obj.mcf)
media_muestral_small_greedy <- mean(small2$f_obj.greedy)
media_muestral_small_mcf <- mean(small2$f_obj.mcf)
t_obs <- (media_muestral_small_greedy - 
media_muestral_small_mcf) / sqrt((var_muestral_small_greedy +
var_muestral_small_mcf) / 10)
p_valor_small_fobj <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor small para la mejora de f_obj")
print(p_valor_small_fobj)

medium2 <- csv[11:20, c(2, 5)]
var_muestral_medium_greedy <- var(medium2$f_obj.greedy)
var_muestral_medium_mcf <- var(medium2$f_obj.mcf)
media_muestral_medium_greedy <- mean(medium2$f_obj.greedy)
media_muestral_medium_mcf <- mean(medium2$f_obj.mcf)
t_obs <- (media_muestral_medium_greedy - 
media_muestral_medium_mcf) / sqrt((var_muestral_medium_greedy +
var_muestral_medium_mcf) / 10)

p_valor_medium_fobj <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor medium para la mejora de f_obj")
print(p_valor_medium_fobj)

large2 <- csv[21:30, c(2, 5)]
var_muestral_large_greedy <- var(large2$f_obj.greedy)
var_muestral_large_mcf <- var(large2$f_obj.mcf)
media_muestral_large_greedy <- mean(large2$f_obj.greedy)
media_muestral_large_mcf <- mean(large2$f_obj.mcf)
t_obs <- (media_muestral_large_greedy - 
media_muestral_large_mcf) / sqrt((var_muestral_large_greedy +
var_muestral_large_mcf) / 10)

p_valor_large_fobj <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor large para la mejora de f_obj")
print(p_valor_large_fobj)

xl2 <- csv[31:40, c(2, 5)]
var_muestral_xl_greedy <- var(xl2$f_obj.greedy)
var_muestral_xl_mcf <- var(xl2$f_obj.mcf)
media_muestral_xl_greedy <- mean(large2$f_obj.greedy)
media_muestral_xl_mcf <- mean(large2$f_obj.mcf)
t_obs <- (media_muestral_xl_greedy - 
media_muestral_xl_mcf) / sqrt((var_muestral_xl_greedy +
var_muestral_xl_mcf) / 10)

p_valor_xl_fobj <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor xl para la mejora de f_obj")
print(p_valor_xl_fobj)




#H0: tiempo.greedy - tiempo.mcf = 0
#H0: tiempo.greedy - tiempo.mcf > 0
small2 <- csv[1:10, c(3, 6)]
var_muestral_small_greedy <- var(small2$tiempo.greedy)
var_muestral_small_mcf <- var(small2$tiempo.mcf)
media_muestral_small_greedy <- mean(small2$tiempo.greedy)
media_muestral_small_mcf <- mean(small2$tiempo.mcf)
t_obs <- (media_muestral_small_greedy - 
media_muestral_small_mcf) / sqrt((var_muestral_small_greedy +
var_muestral_small_mcf) / 10)
p_valor_small_tiempo <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor small para la mejora de tiempo")
print(p_valor_small_tiempo)

medium2 <- csv[11:20, c(3, 6)]
var_muestral_medium_greedy <- var(medium2$tiempo.greedy)
var_muestral_medium_mcf <- var(medium2$tiempo.mcf)
media_muestral_medium_greedy <- mean(medium2$tiempo.greedy)
media_muestral_medium_mcf <- mean(medium2$tiempo.mcf)
t_obs <- (media_muestral_medium_greedy - 
media_muestral_medium_mcf) / sqrt((var_muestral_medium_greedy +
var_muestral_medium_mcf) / 10)

p_valor_medium_tiempo <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor medium para la mejora de tiempo")
print(p_valor_medium_tiempo)

large2 <- csv[21:30, c(3, 6)]
var_muestral_large_greedy <- var(large2$tiempo.greedy)
var_muestral_large_mcf <- var(large2$tiempo.mcf)
media_muestral_large_greedy <- mean(large2$tiempo.greedy)
media_muestral_large_mcf <- mean(large2$tiempo.mcf)
t_obs <- (media_muestral_large_greedy - 
media_muestral_large_mcf) / sqrt((var_muestral_large_greedy +
var_muestral_large_mcf) / 10)

p_valor_large_tiempo <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor large para la mejora de tiemoo")
print(p_valor_large_tiempo)

xl2 <- csv[31:40, c(3, 6)]
var_muestral_xl_greedy <- var(xl2$tiempo.greedy)
var_muestral_xl_mcf <- var(xl2$tiempo.mcf)
media_muestral_xl_greedy <- mean(large2$tiempo.greedy)
media_muestral_xl_mcf <- mean(large2$tiempo.mcf)
t_obs <- (media_muestral_xl_greedy - 
media_muestral_xl_mcf) / sqrt((var_muestral_xl_greedy +
var_muestral_xl_mcf) / 10)

p_valor_xl_tiempo <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor xl para la mejora de tiempo")
print(p_valor_xl_tiempo)





#H0: f_obj.greedy - f_obj.mcf = 0
#H0: f_obj.greedy - f_obj.mcf > 0
small2 <- csv[1:10, c(4, 7)]
var_muestral_small_greedy <- var(small2$dist_max.greedy)
var_muestral_small_mcf <- var(small2$dist_max.mcf)
media_muestral_small_greedy <- mean(small2$dist_max.greedy)
media_muestral_small_mcf <- mean(small2$dist_max.mcf)
t_obs <- (media_muestral_small_greedy - 
media_muestral_small_mcf) / sqrt((var_muestral_small_greedy +
var_muestral_small_mcf) / 10)
p_valor_small_dmax <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor small para la mejora de distancia maxima")
print(p_valor_small_dmax)

medium2 <- csv[11:20, c(4, 7)]
var_muestral_medium_greedy <- var(medium2$dist_max.greedy)
var_muestral_medium_mcf <- var(medium2$dist_max.mcf)
media_muestral_medium_greedy <- mean(medium2$dist_max.greedy)
media_muestral_medium_mcf <- mean(medium2$dist_max.mcf)
t_obs <- (media_muestral_medium_greedy - 
media_muestral_medium_mcf) / sqrt((var_muestral_medium_greedy +
var_muestral_medium_mcf) / 10)

p_valor_medium_dmax <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor medium para la mejora de distancia maxima")
print(p_valor_medium_dmax)

large2 <- csv[21:30, c(4, 7)]
var_muestral_large_greedy <- var(large2$dist_max.greedy)
var_muestral_large_mcf <- var(large2$dist_max.mcf)
media_muestral_large_greedy <- mean(large2$dist_max.greedy)
media_muestral_large_mcf <- mean(large2$dist_max.mcf)
t_obs <- (media_muestral_large_greedy - 
media_muestral_large_mcf) / sqrt((var_muestral_large_greedy +
var_muestral_large_mcf) / 10)

p_valor_large_dmax <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor large para la mejora de distancia maxima")
print(p_valor_large_dmax)

xl2 <- csv[31:40, c(4, 7)]
var_muestral_xl_greedy <- var(xl2$dist_max.greedy)
var_muestral_xl_mcf <- var(xl2$dist_max.mcf)
media_muestral_xl_greedy <- mean(large2$dist_max.greedy)
media_muestral_xl_mcf <- mean(large2$dist_max.mcf)
t_obs <- (media_muestral_xl_greedy - 
media_muestral_xl_mcf) / sqrt((var_muestral_xl_greedy +
var_muestral_xl_mcf) / 10)

p_valor_xl_dmax <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor xl para la mejora de distancia maxima")
print(p_valor_xl_dmax)


#----------------------------------


p_valores_fobj <- c(p_valor_small_fobj, p_valor_medium_fobj, 
p_valor_large_fobj, p_valor_xl_fobj)

p_valores_tiempo <- c(p_valor_small_tiempo, p_valor_medium_tiempo, 
p_valor_large_tiempo, p_valor_xl_tiempo)

p_valores_dmax <- c(p_valor_small_dmax, p_valor_medium_dmax, 
p_valor_large_dmax, p_valor_xl_dmax)

par(mfrow = c(3, 1), mar = c(2, 2, 2, 2))

plot(p_valores_fobj, xlab = "", ylab = "p-valor distancia maxima",
type = "l", lwd = 1, col = colors[1])
points(p_valores_fobj, col = colors[1], cex = 2)

plot(p_valores_tiempo, xlab = "", ylab = "p-valor distancia maxima",
type = "l", lwd = 1, col = colors[2])
points(p_valores_tiempo, col = colors[2], cex = 2)

plot(p_valores_dmax, xlab = "", ylab = "p-valor distancia maxima",
type = "l", lwd = 1, col = colors[3])
points(p_valores_dmax, col = colors[3], cex = 2)

par(mfrow = c(1, 1))

#falta