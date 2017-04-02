

gst.load.file <- function(file.name) {
	read.table(file.name,
		   colClasses=c('integer', 'integer', 'integer', 'numeric', 'numeric', 'numeric', 'numeric', 'numeric'),
		   header=TRUE)
}

gst.display.graphs <- function(d, x.member, x.start_pos, x.vel) {

	x <- d[d$Member == x.member & d$Start == x.start_pos & d$ExpectedOutput == x.vel,]

	par(mfrow = c(1, 2))

	plot(x$Experiment, x$RealOutput, type='l', col='blue', xlab='Trial', ylab='Output')

	m <- min(x$ExpectedOutput, x$RealOutput)
	M <- max(x$ExpectedOutput, x$RealOutput)
	diff <- M - m
	plot(x$Experiment, x$ExpectedOutput, type='l', ylim=c(m - diff/10, M + diff/10), col='red',  xlab='Trial', ylab='Output')
	lines(x$Experiment, x$RealOutput, type='l', col='blue')
}
