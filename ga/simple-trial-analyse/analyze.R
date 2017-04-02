library(ggplot2)

load.d <- function(generation, members) {
	res <- NULL
	for (i in members) {
		file.name <- sprintf("data/results_%.3d_%.3d.csv.gz", generation, i)
		d <- read.table(file.name, header=T)
		res <- rbind(res, d)
	}
	res
}

load.worstbest <- function() {
	read.table('data/worstbest.csv', header=T)
}

draw.one.member <- function(d, member.id, normalize=F) {
	d[d$Member == member.id & d$Experiment >= 302, ] -> d

	unique ( d[, 'ExpectedOutput'] ) -> uniq_eo
	uniq_eo[seq(1, length(uniq_eo), 18)] -> uniq_eo
	union(uniq_eo, c(min(d[, 'ExpectedOutput']), max(d[, 'ExpectedOutput'])) ) -> uniq_eo
	d <- d[d$ExpectedOutput %in% uniq_eo, ]

	d$ExpectedOutputF <- as.factor(d$ExpectedOutput)
	d$StartF <- as.factor(d$Start)


	if (normalize) {
		for (start in unique(d$Start)) {
			for (eo in uniq_eo) {
				rows <- which(d$Start == start & d$ExpectedOutput == eo)
				if (0 == length(rows)) {
					warning(paste('Zero rows for: Start:', start, 'ExpectedOutput:', eo))
					next
				}
				a <- mean(c(d[rows, 'ExpectedOutput'], d[rows, 'RealOutput']))
				D <- max( abs(d[rows, 'ExpectedOutput'] - a), abs(d[rows, 'RealOutput'] - a) )
				d[rows, 'RealOutput'] <- ( d[rows, 'RealOutput'] - a ) / D
				d[rows, 'ExpectedOutput'] <- ( d[rows, 'ExpectedOutput'] - a ) / D
			}
		}
	}

	ggplot(d) +
	  geom_line(aes(y=RealOutput, x=Experiment, colour='Real Output')) +
	  geom_line(aes(y=ExpectedOutput, x=Experiment, colour='Expected Output')) +
#	  geom_text(label='xyz', x=0, y=0, vjust='middle', hjust='middle') +
	  facet_grid(StartF ~ ExpectedOutputF, shrink=F) +
	  theme(axis.ticks=element_blank(), axis.text=element_blank())
}

draw.two.members <- function(d, member.id, member.id2) {
	d[d$Member == member.id2 & d$Experiment >= 302, ] -> d2
	d2 <- data.frame(Experiment=d2$Experiment, RealOutput2=d2$RealOutput, ExpectedOutput=d2$ExpectedOutput, Start=d2$Start)
	d[d$Member == member.id & d$Experiment >= 302, ] -> d
	d <- merge(d, d2, by=c('Experiment', 'ExpectedOutput', 'Start'))

	unique ( d[, 'ExpectedOutput'] ) -> uniq_eo
	uniq_eo[seq(1, length(uniq_eo), 18)] -> uniq_eo
	union(uniq_eo, c(min(d[, 'ExpectedOutput']), max(d[, 'ExpectedOutput'])) ) -> uniq_eo
	d <- d[d$ExpectedOutput %in% uniq_eo, ]

	d$ExpectedOutputF <- as.factor(d$ExpectedOutput)
	d$StartF <- as.factor(d$Start)



	ggplot(d) +
	  geom_line(aes(y=RealOutput, x=Experiment, colour=paste('Real Output', member.id))) +
	  geom_line(aes(y=RealOutput2, x=Experiment, colour=paste('Real Output', member.id2))) +
	  geom_line(aes(y=ExpectedOutput, x=Experiment, colour='Expected Output')) +
#	  geom_text(label='xyz', x=0, y=0, vjust='middle', hjust='middle') +
	  facet_grid(StartF ~ ExpectedOutputF, shrink=F) +
	  theme(axis.ticks=element_blank(), axis.text=element_blank())
}
