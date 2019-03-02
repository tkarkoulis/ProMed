<html>
	<head>
		<title></title>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
		<style>
			body, table, td, table td, table th {
				font-size:12px;
			}
			
			th { background: #eeeeee; text-align:left; }
			
			.header { background-color: #d9d9d9; }
			.data { border:1px solid #ddd; }
			.data td { border-top:1px solid #ddd; padding:4px; }
			.normalfont { font-size:12px; }
			.boldfont { font-weight:bold; }
			.bigfont { font-size:16px; line-height:20px; }
			.spacer { border-bottom: 1px dashed #666; }
		</style>
	</head>
	<body style="font-family:Tahoma; line-height:18px;"> 
	
	<p style="text-align:center; font-size:16px;">
		##title##
	</p>
	
	<p>
		<table cellpadding="2" cellspacing="0" width="100%">
			<tr>
				<td colspan="8" class="bigfont">##fullname##</td>
			</tr>
			
			<tr>
				<td colspan="2" class="boldfont">##ttl_fathername##</td>
				
				<td width="3"></td>
				
				<td colspan="2" class="boldfont">##ttl_email##</td>
				
				<td width="3"></td>
				
				<td colspan="2" class="boldfont">##ttl_bdate##</td>
			</tr>
			
			<tr>
				<td colspan="2" class="normalfont" style="padding-left:10px;">##fathername##</td>
				
				<td width="3"></td>
				
				<td colspan="2" class="normalfont" style="padding-left:10px;">##email##</td>
				
				<td width="3"></td>
				
				<td colspan="2" class="normalfont" style="padding-left:10px;">##bdate##</td>
			</tr>
			
			<tr>
				<td colspan="2" class="boldfont">##ttl_tel1##</td>
				
				<td width="3"></td>
				
				<td colspan="2" class="boldfont">##ttl_insurance##</td>
				
				<td width="3"></td>
				
				<td colspan="2" class="boldfont">##ttl_vat##</td>
			</tr>
			
			<tr>
				<td colspan="2" class="normalfont" style="padding-left:10px;">##tel1##</td>
				
				<td width="3"></td>
				
				<td colspan="2" class="normalfont" style="padding-left:10px;">##insurance##</td>
				
				<td width="3"></td>
				
				<td colspan="2" class="normalfont" style="padding-left:10px;">##vat##</td>
			</tr>
			
			<tr>
				<td colspan="2" class="boldfont">##ttl_tel2##</td>
				
				<td width="3"></td>
				
				<td colspan="2" class="boldfont">##ttl_amka##</td>
				
				<td width="3"></td>
				
				<td colspan="2" class="boldfont">##ttl_gender##</td>
			</tr>

			<tr>
				<td colspan="2" class="normalfont" style="padding-left:10px;">##tel2##</td>
				
				<td width="3"></td>
				
				<td colspan="2" class="normalfont" style="padding-left:10px;">##amka##</td>
				
				<td width="3"></td>
				
				<td colspan="2" class="normalfont" style="padding-left:10px;">##gender##</td>
			</tr>
			
			<tr>
				<td colspan="2" class="boldfont">##ttl_tel3##</td>
				
				<td width="3"></td>
			</tr>
			
			<tr>
				<td colspan="2" class="normalfont" style="padding-left:10px;">##tel3##</td>
				
				<td width="3"></td>
			</tr>
		</table>
	</p>
	
	<p>
		<div class="boldfont">##ttl_address##</div>
		
		<div class="normalfont" style="margin-left:10px;padding-left:20px;">##address##</div>
	</p>
	
	<p>
		<div class="boldfont">##ttl_notes##</div>
		
		<div class="normalfont" style="margin-left:10px;padding-left:20px;">##notes##</div>
	</p>
	
	<div class="spacer"></div>
	
	<p>
		<div class="boldfont">##ttl_history##</div> <br/>
		
		<table width="100%" cellpadding="4" cellspacing="0">
			<tr>
				<td>
					<strong>##ttl_personal_history##</strong> <br/><br/>
					<table width="90%" cellpadding="3" cellspacing="0" style="margin-left:10px;">
						<tr>
							<td>
								<strong>##ttl_surgeries##</strong>
								<p style="margin-left:10px;margin-top:5px;">##surgeries##</p>
							</td>
							<td>
								<strong>##ttl_diseases##</strong>
								<p style="margin-left:10px;margin-top:5px;">##diseases##</p>
							</td>
						</tr>
						<tr>
							<td>
								<strong>##ttl_allergies##</strong>
								<p style="margin-left:10px;margin-top:5px;">##allergies##</p>
							</td>
							<td>
								<strong>##ttl_personal_other##</strong>
								<p style="margin-left:10px;margin-top:5px;">##personal_other##</p>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr>
				<td></td>
			</tr>
			<tr>
				<td>
					<strong>##ttl_habits##</strong><br/><br/>
					
					<table width="90%" cellpadding="3" cellspacing="0" style="margin-left:10px;">
						<tr>
							<td><strong>##ttl_smoking##</strong></td>
							<td><strong>##ttl_sports##</strong></td>
							<td><strong>##ttl_alcohol##</strong></td>
						</tr>
						<tr>
							<td style="padding-left:10px;">##smoking##</td>
							<td style="padding-left:10px;">##sports##</td>
							<td style="padding-left:10px;">##alcohol##</td>
						</tr>
						<tr>
							<td colspan="3"><strong>##ttl_habit_other##</strong></td>
						</tr>
						<tr>
							<td colspan="3" style="padding-left:10px;">##habit_other##</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr>
				<td></td>
			</tr>
			<tr>
				<td>
					<strong>##ttl_family_history##</strong> <br/>
					<p style="padding-left:10px;">##family_history##</p>
				</td>
			</tr>
			<tr>
				<td>
					<strong>##ttl_medications##</strong>
					<p style="padding-left:10px;">##medications##</p>
				</td>
			</tr>
			<tr>
				<td>
					<strong>##ttl_other##</strong>
					<p style="padding-left:10px;">##other##</p>
				</td>
			</tr>
		</table>
	</p>
	
	<div class="spacer"></div>
	
	<p>
		<div class="boldfont">##ttl_apps##</div> <br/>
		
		##apps##
	</p>
	
	<div class="spacer"></div>
	
	<p>
		<div class="boldfont" style="margin-bottom:5px;">##ttl_ops##</div> <br/>
		
		##ops##
	</p>
	
	<div class="spacer"></div>
	
	<p>
		<div class="boldfont" style="margin-bottom:5px;">##ttl_prescriptions##</div> <br/>
		
		##prescriptions##
	</p>
	
	<p>&nbsp;</p>
	
	<p>
		##date## <br/>
		##officename## <br/>
		##officespec## <br/>
		##officeaddress## <br/>
		##officetel##
	</p>
	
	</body>
</html>
