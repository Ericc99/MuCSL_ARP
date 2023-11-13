from rest_framework import serializers

from .models import Chemical, Operator

class ChemicalSerilizer(serializers.ModelSerializer):
    class Meta:
        model = Chemical
        fields = (
            'chemical_id',
            'chemical_name',
            'chemical_info',
            'chemical_quantity'
        )

class OperatorSerilizer(serializers.ModelSerializer):
    class Meta:
        model = Operator
        read_only_fields = (
            'operator_id',
            'operator_name'
        )