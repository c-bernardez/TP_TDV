setwd("/Users/macbookpro/Desktop/TP_TDV")
csv <- read.csv("gap.csv", header = TRUE, sep = ",")
funciones <- csv[2:7]
gap <- csv[8:10]

small <- gap[1:10, ]
medium <- gap[11:20, ]
large <- gap[21:30, ]
xl <- gap[31:40, ]

colors <- c("red", "blue", "green", "purple")

#grafico el valor de las métricas estudiadas con ambos métodos,
#a medida aumenta el tamaño de la entrada
par(mfrow = c(3, 1))

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
     col = colors[1], lwd = "2", ylim = c(0, max(funciones$dist_max.mcf)),
     xlab = "número de archivo",
     ylab = "distancia maxima recorrida en la asignación")
lines(funciones$dist_max.mcf, col = colors
[2], lwd = "2")
legend("topleft", legend = c("greedy", "batching"), col = colors[1:2], lwd = 2)

par(mfrow = c(1, 1))

#guardo los promedios de cada métrica, para cada instancia
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


#grafico la mejora relativa de cada métrica (en promedio)
par(mfrow = c(3, 1))

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



#H0: z_f_obj = 0 (no hay mejora)
#H1: z_f_obj > 0 (hay mejora)
var_muestral <- var(gap$z_f_obj)
media_muestral <- mean(gap$z_f_obj)
t_obs <- (media_muestral - 0) / sqrt(var_muestral / 10)
p_valor <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor total para test de mejora relativa de la funcion objetivo")
print(p_valor)



#H0: z_tiempo = 0 (no hay mejora)
#H1: z_tiempo > 0 (hay mejora)
var_muestral <- var(gap$z_tiempo)
media_muestral <- mean(gap$z_tiempo)
t_obs <- (media_muestral - 0) / sqrt(var_muestral / 10)
p_valor <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor total para test de mejora relativa de la funcion objetivo")
print(p_valor)


#H0: z_d_max = 0 (no hay mejora)
#H1: z_d_max > 0 (hay mejora)
var_muestral <- var(gap$z_d_max)
media_muestral <- mean(gap$z_d_max)
t_obs <- (media_muestral - 0) / sqrt(var_muestral / 10)
p_valor <- pnorm(t_obs, lower.tail = FALSE)
print("p-valor total para test de mejora relativa de la distancia máxima")
print(p_valor)




small2 <- csv[1:10, c(2, 5)]
var_muestral_small_greedy <- var(small2$f_obj.greedy)
var_muestral_small_mcf <- var(small2$f_obj.mcf)
media_muestral_small_greedy <- mean(small2$f_obj.greedy)
media_muestral_small_mcf <- mean(small2$f_obj.mcf)
t_obs <- (media_muestral_small_greedy - 
            media_muestral_small_mcf) / sqrt((var_muestral_small_greedy +
                                                var_muestral_small_mcf) / 10)
print(media_muestral_small_greedy)
print(media_muestral_small_mcf)
p_valor_small <- pnorm(t_obs, lower.tail = FALSE)
print(p_valor_small)

