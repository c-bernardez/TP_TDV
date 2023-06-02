setwd("/Users/macbookpro/Desktop/TP_TDV")
ratios <- read.csv("ratios.csv", header = TRUE, sep = ",")
nuevo <- read.csv("gap_alternativa.csv", header = TRUE, sep = ",")
ratios2 <- read.csv("ratios_alternativa.csv", header = TRUE, sep = ",")


small <- ratios[1:100, ]
small_filtered <- subset(small, is.finite(ratio))

medium <- ratios[101:1100, ]
medium_filtered <- subset(medium, is.finite(ratio))

large <- ratios[1101:3600, ]
large_filtered <- subset(large, is.finite(ratio))

xl <- ratios[3601:8600, ]
xl_filtered <- subset(xl, is.finite(ratio))



ineficientes_small <- sum(small_filtered$ratio > 1)
cantidad_small <- nrow(small_filtered)
print((ineficientes_small / cantidad_small) * 100)

ineficientes_medium <- sum(medium$ratio > 1)
cantidad_medium <- nrow(medium)
print((ineficientes_medium / cantidad_medium) * 100)

ineficientes_large <- sum(large_filtered$ratio > 1)
cantidad_large <- nrow(large_filtered)
print((ineficientes_large / cantidad_large) * 100)

ineficientes_xl <- sum(xl_filtered$ratio > 1)
cantidad_xl <- nrow(xl_filtered)
print((ineficientes_xl / cantidad_xl) * 100)


small_2 <- ratios2[1:100, ]
medium_2 <- ratios2[101:1110, ]
large_2 <- ratios2[1101:3600, ]
xl_2 <- ratios2[3601:8600, ]

ineficientes_small_2 <- sum(small_2$ratio > 1)
cantidad_small_2 <- nrow(small_2)
print((ineficientes_small_2 / cantidad_small_2) * 100)

ineficientes_medium_2 <- sum(medium_2$ratio > 1)
cantidad_medium_2 <- nrow(medium_2)
print((ineficientes_medium_2 / cantidad_medium_2) * 100)

ineficientes_large_2 <- sum(large_2$ratio > 1)
cantidad_large_2 <- nrow(large_2)
print((ineficientes_large_2 / cantidad_large_2) * 100)

ineficientes_xl_2 <- sum(xl_2$ratio > 1, na.rm = TRUE)
cantidad_xl_2 <- nrow(na.omit(xl_2))
print((ineficientes_xl_2 / cantidad_xl_2) * 100)