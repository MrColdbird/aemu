<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/">
		<html>
			<head>
				<title>Prometheus Server Status</title>
				<link rel="stylesheet" type="text/css" href="style.css" />
				<link rel="icon" href="favicon.ico" type="image/x-icon" />
				<meta http-equiv="refresh" content="120" />
			</head>
			<body>
				<div class="headline">
					<xsl:value-of select="./prometheus/@usercount" /> Users are currently playing on <a href="http://www.prometheus.uk.to">Prometheus Online</a>!
				</div>
				<div class="navigation">
					<a class="netlink" href="http://forum.prometheus.uk.to">Community Forum</a>
					<a class="netlink" href="http://code.google.com/p/procfw">Firmware Download</a>
					<a class="netlink" href="http://forum.prometheus.uk.to/viewtopic.php?f=2&amp;t=23">Client Download</a>
					<a class="donatelink" href="https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&amp;hosted_button_id=RJB74KKZAXS3E">Donate via PayPal</a>
				</div>
				<xsl:for-each select="./prometheus/game">
					<xsl:sort select="./@name"/>
					<div class="gametitle">
						<table class="splitter">
							<tr>
								<td class="left">
									<xsl:value-of select="./@name" />
								</td>
								<td class="right">
									<span class="usercount">
										<span class="good"><xsl:value-of select="./@usercount" /> Total</span> -
										<span class="bad">
											<xsl:choose>
												<xsl:when test="./group[@name='Groupless']">
													<xsl:value-of select="./group[@name='Groupless']/@usercount" />
												</xsl:when>
												<xsl:otherwise>0</xsl:otherwise>
											</xsl:choose> Ghosts
										</span>
									</span>
								</td>
							</tr>
						</table>
					</div>
				</xsl:for-each>
				<div class="footer">
					© 2012 Team PRO All Rights Reserved
				</div>
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>
